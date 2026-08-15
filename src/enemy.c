#include "enemy.h"

#include "animation.h"
#include "array.h"
#include "asset.h"
#include "bullet.h"
#include "controller.h"
#include "effect.h"
#include "geometry.h"
#include "sprite.h"

#include <assert.h>
#include <math.h>
#include <raylib.h>
#include <stdlib.h>

static constexpr float FRAME_DURATION = 0.05f;

struct Enemy {
    Sprite *sprite;
    Animation *animation;
    BulletType bullet_type;
    EnemyDirection direction;
    Vector2 position;
    EnemyStatus status;
    EnemyBehavior behavior;
    float behavior_cooldown;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
    float shot_cooldown;
};

static Sprite *GetSprite(const EnemyType type) {
    switch (type) {
    case ENEMY_TYPE_SPECTRA:
        return AssetGetSprite(ASSET_SPACESHIP_SPECTRA);
    }

    return AssetGetSprite(ASSET_SPACESHIP_SPECTRA);
}

static BulletType GetBulletType(const EnemyType type) {
    switch (type) {
    case ENEMY_TYPE_SPECTRA:
        return BULLET_TYPE_BOLT;
    }

    return BULLET_TYPE_BOLT;
}

Enemy *EnemyCreate(const EnemyType type, const int window_width, const int world_height, const int world_border) {
    Sprite *sprite = GetSprite(type);
    if (sprite == NULL) {
        return NULL;
    }

    Animation *animation = AnimationCreate(sprite, FRAME_DURATION);
    if (animation == NULL) {
        SpriteDestroy(sprite);

        return NULL;
    }

    Enemy *enemy = malloc(sizeof(*enemy));
    if (enemy == NULL) {
        AnimationDestroy(animation);
        SpriteDestroy(sprite);

        return NULL;
    }

    *enemy = (Enemy){.sprite = sprite,
        .animation = animation,
        .bullet_type = GetBulletType(type),
        .direction = ENEMY_DIRECTION_LEFT,
        .status = ENEMY_STATUS_NORMAL,
        .behavior = ENEMY_BEHAVIOR_PURSUIT,
        .shot_cooldown = ENEMY_SHOT_COOLDOWN};

    Rectangle bounds = EnemyGetBounds(enemy);

    enemy->position = (Vector2){window_width, GetRandomValue(world_border, world_height - bounds.height)};

    return enemy;
}

void EnemyDestroy(Enemy *enemy) {
    assert(enemy);

    for (int i = 0; i < enemy->bullet_count; i++) {
        (void)EnemyRemoveBullet(enemy, i);

        i--;
    }

    AnimationDestroy(enemy->animation);
    SpriteDestroy(enemy->sprite);
    free(enemy);
}

float EnemyGetWidth(const Enemy *enemy) {
    assert(enemy);

    Frame frame = AnimationGetCurrentFrame(enemy->animation);

    return frame.width * ENEMY_SCALE;
}

float EnemyGetHeight(const Enemy *enemy) {
    assert(enemy);

    Frame frame = AnimationGetCurrentFrame(enemy->animation);

    return frame.height * ENEMY_SCALE;
}

Vector2 EnemyGetPosition(const Enemy *enemy) {
    assert(enemy);

    return enemy->position;
}

Rectangle EnemyGetBounds(const Enemy *enemy) {
    assert(enemy);

    return (Rectangle){
        .x = enemy->position.x,
        .y = enemy->position.y,
        .width = EnemyGetWidth(enemy),
        .height = EnemyGetHeight(enemy),
    };
}

Vector2 EnemyGetCenterPosition(const Enemy *enemy) {
    assert(enemy);

    return GeometryGetCenterFromRectangle(EnemyGetBounds(enemy));
}

EnemyStatus EnemyGetStatus(const Enemy *enemy) {
    assert(enemy);

    return enemy->status;
}

Bullet *EnemyGetBullet(Enemy *enemy, const int bullet_index) {
    assert(enemy);

    if (bullet_index < 0 || bullet_index >= enemy->bullet_count) {
        return NULL;
    }

    return enemy->bullets[bullet_index];
}

int EnemyGetBulletCount(const Enemy *enemy) {
    assert(enemy);

    return enemy->bullet_count;
}

bool EnemyRemoveBullet(Enemy *enemy, const int bullet_index) {
    assert(enemy);

    if (bullet_index < 0 || bullet_index >= enemy->bullet_count) {
        return false;
    }

    Bullet *bullet = enemy->bullets[bullet_index];
    BulletDestroy(bullet);

    for (int i = bullet_index; i < enemy->bullet_count - 1; i++) {
        enemy->bullets[i] = enemy->bullets[i + 1];
    }

    enemy->bullet_count--;

    return true;
}

bool EnemyRemoveBullets(Enemy *enemy, int *bullet_indexes, const int bullet_index_count) {
    assert(enemy);

    qsort(bullet_indexes, bullet_index_count, sizeof(*bullet_indexes), ArrayCompareIntegerAscending);

    bool all_succeeded = true;

    for (int i = bullet_index_count - 1; i >= 0; i--) {
        if (!EnemyRemoveBullet(enemy, bullet_indexes[i])) {
            TraceLog(LOG_ERROR, "Failed to remove bullet at index %d", bullet_indexes[i]);
            all_succeeded = false;
        }
    }

    return all_succeeded;
}

ControllerInput Think(Enemy *enemy, const int window_height, const int world_width, const Vector2 player_position,
    const Vector2 player_center_position) {
    Rectangle bounds = EnemyGetBounds(enemy);
    Vector2 enemy_center_position = EnemyGetCenterPosition(enemy);
    float player_distance_y = fabsf(player_center_position.y - enemy_center_position.y);
    ControllerInput input = {};

    switch (enemy->behavior) {
    case ENEMY_BEHAVIOR_PURSUIT:
        if (enemy->position.x > (world_width - bounds.width)) {
            input.left_held = true;
        }

        if (enemy->position.y > player_position.y) {
            input.up_held = true;
        } else {
            input.down_held = true;
        }

        break;
    case ENEMY_BEHAVIOR_RETREAT:
        if (enemy_center_position.y > player_center_position.y) {
            if (enemy->position.y < (window_height + bounds.height)) {
                input.down_held = true;
            }
        } else {
            if (enemy->position.y > (bounds.height * -1)) {
                input.up_held = true;
            }
        }
        break;
    }

    if (player_distance_y <= ENEMY_ATTACK_DISTANCE) {
        input.shoot_pressed = true;
    }

    return input;
}

static void Move(Enemy *enemy, const ControllerInput input, const float delta) {
    float move_step = ENEMY_SPEED * delta;

    if (input.left_held && !input.right_held) {
        enemy->position.x -= move_step;
    }

    if (input.up_held && !input.down_held) {
        enemy->position.y -= move_step;
    }

    if (input.right_held && !input.left_held) {
        enemy->position.x += move_step;
    }

    if (input.down_held && !input.up_held) {
        enemy->position.y += move_step;
    }
}

static void Shoot(Enemy *enemy) {
    if ((enemy->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return;
    }

    Bullet *bullet = BulletCreate(enemy->bullet_type, BULLET_DIRECTION_LEFT, EnemyGetCenterPosition(enemy));
    if (bullet == NULL) {
        TraceLog(LOG_ERROR, "Failed to create enemy bullet");

        return;
    }

    enemy->bullets[enemy->bullet_count++] = bullet;
}

void EnemyTakeDamage(Enemy *enemy) {
    assert(enemy);

    switch (enemy->status) {
    case ENEMY_STATUS_NORMAL:
        enemy->status = ENEMY_STATUS_DAMAGED;
        enemy->behavior = ENEMY_BEHAVIOR_RETREAT;
        enemy->behavior_cooldown = ENEMY_BEHAVIOR_COOLDOWN;
        break;
    case ENEMY_STATUS_DAMAGED:
        enemy->status = ENEMY_STATUS_DESTROYED;

        AnimationSetFrame(enemy->animation, 1);
        break;
    case ENEMY_STATUS_DESTROYED:
    case ENEMY_STATUS_EXPLODED:
        break;
    }
}

void EnemyUpdate(Enemy *enemy, const int window_height, const int world_width, const Vector2 player_position,
    const Vector2 player_center_position, const float delta) {
    assert(enemy);

    if (enemy->status == ENEMY_STATUS_DESTROYED) {
        Vector2 previous_center_position = EnemyGetCenterPosition(enemy);

        AnimationUpdate(enemy->animation, delta);

        Vector2 centered_position =
            GeometryGetCenteredPosition(previous_center_position, EnemyGetWidth(enemy), EnemyGetHeight(enemy));

        enemy->position = centered_position;

        if (AnimationIsFinished(enemy->animation)) {
            enemy->status = ENEMY_STATUS_EXPLODED;
        }

        return;
    }

    enemy->behavior_cooldown -= delta;
    enemy->shot_cooldown -= delta;

    if (enemy->behavior_cooldown <= 0.0f) {
        enemy->behavior = ENEMY_BEHAVIOR_PURSUIT;
    }

    ControllerInput input = Think(enemy, window_height, world_width, player_position, player_center_position);

    Move(enemy, input, delta);

    if (input.shoot_pressed && enemy->shot_cooldown <= 0.0f) {
        Shoot(enemy);

        enemy->shot_cooldown = ENEMY_SHOT_COOLDOWN;
    }
}

void EnemyDraw(const Enemy *enemy) {
    assert(enemy);

    Frame frame = AnimationGetCurrentFrame(enemy->animation);

    Rectangle source = {.x = frame.x, .y = frame.y, .width = frame.width * enemy->direction, .height = frame.height};
    Rectangle destination = {enemy->position.x, enemy->position.y, EnemyGetWidth(enemy), EnemyGetHeight(enemy)};

    Color color = WHITE;
    if (enemy->status == ENEMY_STATUS_DAMAGED && enemy->behavior == ENEMY_BEHAVIOR_RETREAT) {
        color = EffectBlinkColor(10.0f);
    }

    DrawTexturePro(SpriteGetTexture(enemy->sprite), source, destination, (Vector2){}, 0, color);
}
