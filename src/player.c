#include "player.h"

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

void PlayerDraw(const Player *player) { DrawRectangleV(player->position, player->size, BLACK); }
