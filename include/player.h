#pragma once
#include "asset.h"
#include "bullet.h"
#include <raylib.h>

constexpr int MAX_PLAYERS = 2;
constexpr float PLAYER_SPEED = 400.0f;
constexpr float PLAYER_SCALE = 1.0f;

typedef enum { PLAYER_TYPE_VIPER } PlayerType;

typedef struct Player Player;

Player *PlayerCreate(const PlayerType type);
void PlayerDestroy(Player *player);

float PlayerGetWidth(const Player *player);
float PlayerGetHeight(const Player *player);
Vector2 *PlayerGetPosition(Player *player);
TextureSize PlayerGetSize(const Player *player);
Bullet *PlayerGetBullet(Player *player, const int index);
int PlayerGetBulletCount(const Player *player);

void PlayerUpdate(Player *player, const float delta);
void PlayerDraw(const Player *player);
bool PlayerShot(Player *player);
void PlayerSpliceBullet(Player *player, const int index);
