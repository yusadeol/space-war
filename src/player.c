#include "player.h"
#include "asset.h"
#include "bullet.h"
#include <raylib.h>
#include <stdlib.h>

struct Player {
    Texture2D texture;
    Vector2 position;
    Bullet bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
};

static Texture2D PlayerTexture(SpaceshipType type) {
    switch (type) {
    case SPACESHIP_TYPE_VIPER:
        return *AssetGetTexture(TEXTURE_SPACESHIP_VIPER);
    }

    return (Texture2D){};
}

Player *PlayerCreate(SpaceshipType type) {
    Player *player = malloc(sizeof(Player));

    *player = (Player){.texture = PlayerTexture(type)};

    return player;
}

void PlayerDestroy(Player *player) {
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

TextureSize PlayerGetSize(const Player *player) {
    return (TextureSize){player->texture.width, player->texture.height};
}

Bullet *PlayerGetBullet(Player *player, int index) {
    return &player->bullets[index];
}

int PlayerGetBulletCount(const Player *player) {
    return player->bullet_count;
}

void PlayerUpdate(Player *player, float delta) {
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

void PlayerShot(Player *player) {
    Vector2 position = {player->position.x + (PlayerGetWidth(player) / 2),
                        player->position.y + (PlayerGetHeight(player) / 2)};

    player->bullets[player->bullet_count++] =
        BulletCreate(BULLET_TYPE_PULSE, position);
}

void PlayerSpliceBullet(Player *player, int index) {
    for (int i = index; i < player->bullet_count - 1; i++) {
        player->bullets[i] = player->bullets[i + 1];
    }

    player->bullet_count--;
}
