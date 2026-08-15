#include "player.h"

#include "animation.h"
#include "array.h"
#include "asset.h"
#include "bullet.h"
#include "geometry.h"
#include "sprite.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static constexpr float FRAME_DURATION = 0.05f;

struct Player {
    Sprite *sprite;
    Animation *animation;
    BulletType bullet_type;
    PlayerDirection direction;
    Vector2 position;
    PlayerStatus status;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
    int kill_count;
};

static Sprite *GetSprite(const PlayerType type) {
    switch (type) {
    case PLAYER_TYPE_VIPER:
        return AssetGetSprite(TEXTURE_SPACESHIP_VIPER);
    case PLAYER_TYPE_RAPTOR:
        return AssetGetSprite(TEXTURE_SPACESHIP_RAPTOR);
    }

    return AssetGetSprite(TEXTURE_SPACESHIP_VIPER);
}

static BulletType GetBulletType(const PlayerType type) {
    switch (type) {
    case PLAYER_TYPE_VIPER:
        return BULLET_TYPE_PULSE;
    case PLAYER_TYPE_RAPTOR:
        return BULLET_TYPE_HAMMER;
    }

    return BULLET_TYPE_PULSE;
}

Player *PlayerCreate(const PlayerType type) {
    Sprite *sprite = GetSprite(type);
    if (sprite == NULL) {
        return NULL;
    }

    Animation *animation = AnimationCreate(sprite, FRAME_DURATION);
    if (animation == NULL) {
        SpriteDestroy(sprite);

        return NULL;
    }

    Player *player = malloc(sizeof(*player));

    if (player == NULL) {
        AnimationDestroy(animation);
        SpriteDestroy(sprite);

        return NULL;
    }

    *player = (Player){
        .sprite = sprite,
        .animation = animation,
        .bullet_type = GetBulletType(type),
        .direction = PLAYER_DIRECTION_RIGHT,
        .status = PLAYER_STATUS_NORMAL,
    };

    return player;
}

void PlayerDestroy(Player *player) {
    assert(player);

    for (int i = 0; i < player->bullet_count; i++) {
        (void)PlayerRemoveBullet(player, i);

        i--;
    }

    AnimationDestroy(player->animation);
    SpriteDestroy(player->sprite);
    free(player);
}

float PlayerGetWidth(const Player *player) {
    assert(player);

    Frame frame = AnimationGetCurrentFrame(player->animation);

    return frame.width * PLAYER_SCALE;
}

float PlayerGetHeight(const Player *player) {
    assert(player);

    Frame frame = AnimationGetCurrentFrame(player->animation);

    return frame.height * PLAYER_SCALE;
}

void PlayerSetPosition(Player *player, const Vector2 position) {
    assert(player);

    player->position = position;
}

Vector2 PlayerGetPosition(const Player *player) {
    assert(player);

    return player->position;
}

Rectangle PlayerGetBounds(const Player *player) {
    assert(player);

    return (Rectangle){
        .x = player->position.x,
        .y = player->position.y,
        .width = PlayerGetWidth(player),
        .height = PlayerGetHeight(player),
    };
}

Vector2 PlayerGetCenterPosition(const Player *player) {
    assert(player);

    return GeometryGetCenterFromRectangle(PlayerGetBounds(player));
}

PlayerStatus PlayerGetStatus(const Player *player) {
    assert(player);

    return player->status;
}

Bullet *PlayerGetBullet(Player *player, const int bullet_index) {
    assert(player);

    if (bullet_index < 0 || bullet_index >= player->bullet_count) {
        return NULL;
    }

    return player->bullets[bullet_index];
}

int PlayerGetBulletCount(const Player *player) {
    assert(player);

    return player->bullet_count;
}

bool PlayerRemoveBullet(Player *player, const int bullet_index) {
    assert(player);

    if (bullet_index < 0 || bullet_index >= player->bullet_count) {
        return false;
    }

    Bullet *bullet = player->bullets[bullet_index];
    BulletDestroy(bullet);

    for (int i = bullet_index; i < player->bullet_count - 1; i++) {
        player->bullets[i] = player->bullets[i + 1];
    }

    player->bullet_count--;

    return true;
}

bool PlayerRemoveBullets(Player *player, int *bullet_indexes, const int bullet_index_count) {
    assert(player);

    qsort(bullet_indexes, bullet_index_count, sizeof(*bullet_indexes), ArrayCompareIntegerAscending);

    bool all_ok = true;

    for (int i = bullet_index_count - 1; i >= 0; i--) {
        if (!PlayerRemoveBullet(player, bullet_indexes[i])) {
            TraceLog(LOG_ERROR, "Failed to remove bullet at index %d", bullet_indexes[i]);
            all_ok = false;
        }
    }

    return all_ok;
}

void PlayerIncrementKillCount(Player *player) {
    assert(player);

    player->kill_count++;
}

int PlayerGetKillCount(const Player *player) {
    assert(player);

    return player->kill_count;
}

static void Move(Player *player, const ControllerInput input, const float delta) {
    float move_step = PLAYER_SPEED * delta;

    if (input.left && !input.right) {
        player->position.x -= move_step;
    }

    if (input.up && !input.down) {
        player->position.y -= move_step;
    }

    if (input.right && !input.left) {
        player->position.x += move_step;
    }

    if (input.down && !input.up) {
        player->position.y += move_step;
    }
}

static void Shoot(Player *player) {
    if ((player->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return;
    }

    Bullet *bullet = BulletCreate(player->bullet_type, BULLET_DIRECTION_RIGHT, PlayerGetCenterPosition(player));
    if (bullet == NULL) {
        TraceLog(LOG_ERROR, "Failed to create player bullet");

        return;
    }

    player->bullets[player->bullet_count++] = bullet;
}

void PlayerTakeDamage(Player *player) {
    assert(player);

    switch (player->status) {
    case PLAYER_STATUS_NORMAL:
        player->status = PLAYER_STATUS_DESTROYED;

        AnimationSetFrame(player->animation, 1);
        break;
    case PLAYER_STATUS_DESTROYED:
    case PLAYER_STATUS_EXPLODED:
        break;
    }
}

void PlayerUpdate(Player *player, const ControllerInput input, const float delta) {
    assert(player);

    if (player->status == PLAYER_STATUS_DESTROYED) {
        Vector2 previous_center_position = PlayerGetCenterPosition(player);

        AnimationUpdate(player->animation, delta);

        Vector2 centered_position =
            GeometryGetCenteredPosition(previous_center_position, PlayerGetWidth(player), PlayerGetHeight(player));

        player->position = centered_position;

        if (AnimationIsFinished(player->animation)) {
            player->status = PLAYER_STATUS_EXPLODED;
        }

        return;
    }

    Move(player, input, delta);

    if (input.shoot) {
        Shoot(player);
    }
}

void PlayerDraw(const Player *player) {
    assert(player);

    Frame frame = AnimationGetCurrentFrame(player->animation);

    Rectangle source = {.x = frame.x, .y = frame.y, .width = frame.width * player->direction, .height = frame.height};
    Rectangle destination = {player->position.x, player->position.y, PlayerGetWidth(player), PlayerGetHeight(player)};

    DrawTexturePro(SpriteGetTexture(player->sprite), source, destination, (Vector2){}, 0, WHITE);
}
