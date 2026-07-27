#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    Vector2 position;
    Vector2 size;
    float speed;
} Player;

void PlayerUpdate(Player *player, float delta);
void PlayerDraw(const Player *player);

#endif
