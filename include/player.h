#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct Player {
    Texture2D texture;
    Vector2 position;
    Vector2 scale;
    float speed;
} Player;

void PlayerUpdate(Player *player, float delta);
float PlayerWidth(const Player *player);
float PlayerHeight(const Player *player);
void PlayerDraw(const Player *player);

#endif
