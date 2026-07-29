#include "player.h"
#include "bullet.h"
#include <raylib.h>

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

float PlayerWidth(const Player *player) {
    return player->texture.width * PLAYER_SCALE;
}

float PlayerHeight(const Player *player) {
    return player->texture.height * PLAYER_SCALE;
}

void PlayerDraw(const Player *player) {

    Rectangle source = {.width = player->texture.width,
                        .height = player->texture.height};
    Rectangle destination = {player->position.x, player->position.y,
                             PlayerWidth(player), PlayerHeight(player)};

    DrawTexturePro(player->texture, source, destination, (Vector2){}, 0, WHITE);
}

void PlayerShot(Player *player) {
    Vector2 position = {player->position.x + (PlayerWidth(player) / 2),
                        player->position.y + (PlayerHeight(player) / 2)};

    player->bullets[player->bullet_count++] =
        BulletCreate(BULLET_TYPE_PULSE, position);
}
