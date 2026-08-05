#include "enemy.h"
#include "asset.h"
#include "bullet.h"
#include "controller.h"
#include "geometry.h"
#include <math.h>
#include <raylib.h>
#include <stdlib.h>

struct Enemy {
    Texture2D texture;
    Vector2 position;
    EnemyStatus status;
    EnemyBehavior behavior;
    float behavior_cooldown;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
    float shot_cooldown;
};

static Texture2D GetTexture(const EnemyType type) {
    switch (type) {
    case ENEMY_TYPE_SPECTRA:
        return *AssetGetTexture(TEXTURE_SPACESHIP_SPECTRA);
    }

    return (Texture2D){};
}

Enemy *EnemyCreate(
    const EnemyType type, const int window_width, const int world_height,
    const int world_border) {
    Enemy *enemy = malloc(sizeof(*enemy));

    if (enemy == NULL) {
        return NULL;
    }

    *enemy = (Enemy){.texture = GetTexture(type),
                     .status = ENEMY_STATUS_NORMAL,
                     .behavior = ENEMY_BEHAVIOR_PURSUIT,
                     .shot_cooldown = ENEMY_SHOT_COOLDOWN};

    Rectangle bounds = EnemyGetBounds(enemy);

    enemy->position =
        (Vector2){window_width,
                  GetRandomValue(world_border, world_height - bounds.height)};

    return enemy;
}

void EnemyDestroy(Enemy *enemy) {
    for (int i = 0; i < enemy->bullet_count; i++) {
        Bullet *bullet = enemy->bullets[i];
        BulletDestroy(bullet);
    }

    free(enemy);
}

float EnemyGetWidth(const Enemy *enemy) {
    return enemy->texture.width * ENEMY_SCALE;
}

float EnemyGetHeight(const Enemy *enemy) {
    return enemy->texture.height * ENEMY_SCALE;
}

Vector2 *EnemyGetPosition(Enemy *enemy) {
    return &enemy->position;
}

EnemyStatus EnemyGetStatus(const Enemy *enemy) {
    return enemy->status;
}

Rectangle EnemyGetBounds(const Enemy *enemy) {
    return (Rectangle){
        .x = enemy->position.x,
        .y = enemy->position.y,
        .width = EnemyGetWidth(enemy),
        .height = EnemyGetHeight(enemy),
    };
}

Vector2 EnemyGetCenterPosition(const Enemy *enemy) {
    return GeometryGetCenterFromRectangle(EnemyGetBounds(enemy));
}

Bullet *EnemyGetBullet(Enemy *enemy, const int index) {
    return enemy->bullets[index];
}

int EnemyGetBulletCount(const Enemy *enemy) {
    return enemy->bullet_count;
}

ControllerInput Think(
    Enemy *enemy, const int window_height, const int world_width,
    const Vector2 player_position, const Vector2 player_center_position) {
    Rectangle bounds = EnemyGetBounds(enemy);
    Vector2 enemy_center_position = EnemyGetCenterPosition(enemy);
    float player_distance_y =
        fabsf(player_center_position.y - enemy_center_position.y);
    ControllerInput input = {};

    switch (enemy->behavior) {
    case ENEMY_BEHAVIOR_PURSUIT:
        if (enemy->position.x > (world_width - bounds.width)) {
            input.left = true;
        }

        if (enemy->position.y > player_position.y) {
            input.up = true;
        } else {
            input.down = true;
        }

        break;
    case ENEMY_BEHAVIOR_RETREAT:
        if (enemy_center_position.y > player_center_position.y) {
            if (enemy->position.y < (window_height + bounds.height)) {
                input.down = true;
            }
        } else {
            if (enemy->position.y > (bounds.height * -1)) {
                input.up = true;
            }
        }
        break;
    }

    if (player_distance_y <= ENEMY_ATTACK_DISTANCE) {
        input.shoot = true;
    }

    return input;
}

static void Move(Enemy *enemy, const ControllerInput input, const float delta) {
    float move_step = ENEMY_SPEED * delta;

    if (input.left && !input.right) {
        enemy->position.x -= move_step;
    }

    if (input.up && !input.down) {
        enemy->position.y -= move_step;
    }

    if (input.right && !input.left) {
        enemy->position.x += move_step;
    }

    if (input.down && !input.up) {
        enemy->position.y += move_step;
    }
}

static void Shoot(Enemy *enemy) {
    if ((enemy->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return;
    }

    enemy->bullets[enemy->bullet_count++] = BulletCreate(
        BULLET_TYPE_BOLT, BULLET_DIRECTION_LEFT, EnemyGetBounds(enemy));
}

void EnemyUpdate(
    Enemy *enemy, const int window_height, const int world_width,
    const Vector2 player_position, const Vector2 player_center_position,
    const float delta) {

    enemy->behavior_cooldown -= delta;
    enemy->shot_cooldown -= delta;

    if (enemy->behavior_cooldown <= 0.0f) {
        enemy->behavior = ENEMY_BEHAVIOR_PURSUIT;
    }

    ControllerInput input = Think(
        enemy, window_height, world_width, player_position,
        player_center_position);

    Move(enemy, input, delta);

    if (input.shoot && enemy->shot_cooldown <= 0.0f) {
        Shoot(enemy);

        enemy->shot_cooldown = ENEMY_SHOT_COOLDOWN;
    }
}

static Color BlinkColor(float frequency) {
    int blink = (int)(GetTime() * frequency);
    bool visible = (blink % 2) != 0;

    return visible ? WHITE : BLANK;
}

void EnemyDraw(const Enemy *enemy) {

    Rectangle source = {.width = -enemy->texture.width,
                        .height = enemy->texture.height};
    Rectangle destination = {enemy->position.x, enemy->position.y,
                             EnemyGetWidth(enemy), EnemyGetHeight(enemy)};

    DrawTexturePro(
        enemy->texture, source, destination, (Vector2){}, 0,
        enemy->status == ENEMY_STATUS_DAMAGED &&
                enemy->behavior == ENEMY_BEHAVIOR_RETREAT
            ? BlinkColor(10.0f)
            : WHITE);
}

void EnemyTakeDamage(Enemy *enemy) {
    if (enemy->status == ENEMY_STATUS_DESTROYED) {
        return;
    }

    if (enemy->status == ENEMY_STATUS_DAMAGED) {
        enemy->status = ENEMY_STATUS_DESTROYED;

        return;
    }

    enemy->status = ENEMY_STATUS_DAMAGED;
    enemy->behavior = ENEMY_BEHAVIOR_RETREAT;
    enemy->behavior_cooldown = ENEMY_BEHAVIOR_COOLDOWN;
}

void EnemyRemoveBullet(Enemy *enemy, const int index) {
    Bullet *bullet = enemy->bullets[index];
    BulletDestroy(bullet);

    for (int i = index; i < enemy->bullet_count - 1; i++) {
        enemy->bullets[i] = enemy->bullets[i + 1];
    }

    enemy->bullet_count--;
}
