#include "player.h"
#include "asset.h"
#include "bullet.h"
#include "geometry.h"
#include <raylib.h>
#include <stdlib.h>

struct Player {
    Texture2D texture;
    Vector2 position;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
};

static Texture2D PlayerTexture(const PlayerType type) {
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

    *player = (Player){.texture = PlayerTexture(type)};

    return player;
}

void PlayerDestroy(Player *player) {
    for (int i = 0; i < player->bullet_count; i++) {
        Bullet *bullet = player->bullets[i];
        BulletDestroy(bullet);
    }

    free(player);
}

float PlayerGetWidth(const Player *player) {
    return player->texture.width * PLAYER_SCALE;
}

float PlayerGetHeight(const Player *player) {
    return player->texture.height * PLAYER_SCALE;
}

Vector2 *PlayerGetPosition(Player *player) {
    return &player->position;
}

TextureSize PlayerGetTextureSize(const Player *player) {
    return (TextureSize){player->texture.width, player->texture.height};
}

Rectangle PlayerGetBounds(const Player *player) {
    return (Rectangle){
        .x = player->position.x,
        .y = player->position.y,
        .width = PlayerGetWidth(player),
        .height = PlayerGetHeight(player),
    };
}

Vector2 PlayerGetCenterPosition(const Player *player) {
    return RectangleCenter(PlayerGetBounds(player));
}

Bullet *PlayerGetBullet(Player *player, const int index) {
    return player->bullets[index];
}

int PlayerGetBulletCount(const Player *player) {
    return player->bullet_count;
}

void PlayerUpdate(Player *player, const float delta) {
    float move_step = PLAYER_SPEED * delta;

    if (IsKeyDown(KEY_LEFT)) {
        player->position.x -= move_step;
    }

    if (IsKeyDown(KEY_UP)) {
        player->position.y -= move_step;
    }

    if (IsKeyDown(KEY_RIGHT)) {
        player->position.x += move_step;
    }

    if (IsKeyDown(KEY_DOWN)) {
        player->position.y += move_step;
    }

    if (IsKeyPressed(KEY_SPACE)) {
        PlayerShot(player);
    }
}

void PlayerDraw(const Player *player) {

    Rectangle source = {.width = player->texture.width,
                        .height = player->texture.height};
    Rectangle destination = {player->position.x, player->position.y,
                             PlayerGetWidth(player), PlayerGetHeight(player)};

    DrawTexturePro(player->texture, source, destination, (Vector2){}, 0, WHITE);
}

bool PlayerShot(Player *player) {
    if ((player->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return false;
    }

    player->bullets[player->bullet_count++] = BulletCreate(
        BULLET_TYPE_PULSE, BULLET_DIRECTION_RIGHT,
        PlayerGetCenterPosition(player));

    return true;
}

void PlayerSpliceBullet(Player *player, const int index) {
    Bullet *bullet = player->bullets[index];
    BulletDestroy(bullet);

    for (int i = index; i < player->bullet_count - 1; i++) {
        player->bullets[i] = player->bullets[i + 1];
    }

    player->bullet_count--;
}
