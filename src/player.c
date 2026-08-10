#include "player.h"

#include "array.h"
#include "asset.h"
#include "bullet.h"
#include "geometry.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Player {
    Texture2D texture;
    Vector2 position;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
    int kill_count;
};

static Texture2D GetTexture(const PlayerType type) {
    switch (type) {
    case PLAYER_TYPE_VIPER:
        return *AssetGetTexture(TEXTURE_SPACESHIP_VIPER);
    }

    return (Texture2D){};
}

Player *PlayerCreate(const PlayerType type) {
    Player *player = malloc(sizeof(*player));

    if (player == NULL) {
        return NULL;
    }

    *player = (Player){.texture = GetTexture(type)};

    return player;
}

void PlayerDestroy(Player *player) {
    assert(player);

    for (int i = 0; i < player->bullet_count; i++) {
        PlayerRemoveBullet(player, i);

        i--;
    }

    free(player);
}

float PlayerGetWidth(const Player *player) {
    assert(player);

    return player->texture.width * PLAYER_SCALE;
}

float PlayerGetHeight(const Player *player) {
    assert(player);

    return player->texture.height * PLAYER_SCALE;
}

Vector2 *PlayerGetPosition(Player *player) {
    assert(player);

    return &player->position;
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

void PlayerRemoveBullet(Player *player, const int bullet_index) {
    assert(player);

    if (bullet_index < 0 || bullet_index >= player->bullet_count) {
        return;
    }

    Bullet *bullet = player->bullets[bullet_index];
    BulletDestroy(bullet);

    for (int i = bullet_index; i < player->bullet_count - 1; i++) {
        player->bullets[i] = player->bullets[i + 1];
    }

    player->bullet_count--;
}

void PlayerRemoveBullets(Player *player, int *bullet_indexes, const int bullet_index_count) {
    qsort(bullet_indexes, bullet_index_count, sizeof(*bullet_indexes), ArrayCompareIntegerAscending);

    for (int i = bullet_index_count - 1; i >= 0; i--) {
        PlayerRemoveBullet(player, bullet_indexes[i]);
    }
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

    player->bullets[player->bullet_count++] =
        BulletCreate(BULLET_TYPE_PULSE, BULLET_DIRECTION_RIGHT, PlayerGetBounds(player));
}

void PlayerUpdate(Player *player, const ControllerInput input, const float delta) {

    assert(player);

    Move(player, input, delta);

    if (input.shoot) {
        Shoot(player);
    }
}

void PlayerDraw(const Player *player) {
    assert(player);

    Rectangle source = {.width = player->texture.width, .height = player->texture.height};
    Rectangle destination = {player->position.x, player->position.y, PlayerGetWidth(player), PlayerGetHeight(player)};

    DrawTexturePro(player->texture, source, destination, (Vector2){}, 0, WHITE);
}

void PlayerIncrementKillCountByAmount(Player *player, const int amount) {
    assert(player);

    player->kill_count += amount;
}
