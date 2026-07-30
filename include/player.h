#ifndef PLAYER_H
#define PLAYER_H

#include "bullet.h"
#include "raylib.h"

constexpr int MAX_PLAYERS = 2;
constexpr float PLAYER_SPEED = 400.0f;
constexpr float PLAYER_SCALE = 1.0f;

typedef struct Player {
    Texture2D texture;
    Vector2 position;
    Bullet bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
} Player;

void PlayerUpdate(Player *player, float delta);
float PlayerWidth(const Player *player);
float PlayerHeight(const Player *player);
void PlayerDraw(const Player *player);
void PlayerShot(Player *player);
void PlayerSpliceBullet(Player *player, int index);

#endif
