#include "player.h"
#include <raylib.h>

void PlayerUpdate(Player *player, float delta) {
    float move_step = player->speed * delta;

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
}

float PlayerWidth(const Player *player) { return player->texture.width * player->scale.x; }

float PlayerHeight(const Player *player) { return player->texture.height * player->scale.y; }

void PlayerDraw(const Player *player) {

    Rectangle source = {.width = player->texture.width, .height = player->texture.height};

    Rectangle dest = {player->position.x, player->position.y, PlayerWidth(player), PlayerHeight(player)};

    Vector2 origin = {};

    DrawTexturePro(player->texture, source, dest, origin, 0, WHITE);
}
