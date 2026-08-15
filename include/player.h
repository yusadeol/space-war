#pragma once
#include "bullet.h"
#include "controller.h"

#include <raylib.h>

constexpr int MAX_PLAYERS = 2;
constexpr float PLAYER_SPEED = 400.0f;
constexpr float PLAYER_SCALE = 1.0f;

typedef enum { PLAYER_TYPE_VIPER, PLAYER_TYPE_RAPTOR } PlayerType;
typedef enum { PLAYER_DIRECTION_RIGHT = 1, PLAYER_DIRECTION_LEFT = -1 } PlayerDirection;
typedef enum { PLAYER_STATUS_NORMAL, PLAYER_STATUS_DESTROYED, PLAYER_STATUS_EXPLODED } PlayerStatus;

typedef struct Player Player;

Player *PlayerCreate(const PlayerType type);
void PlayerDestroy(Player *player);

float PlayerGetWidth(const Player *player);
float PlayerGetHeight(const Player *player);

void PlayerSetPosition(Player *player, const Vector2 position);
Vector2 PlayerGetPosition(const Player *player);
Rectangle PlayerGetBounds(const Player *player);
Vector2 PlayerGetCenterPosition(const Player *player);

PlayerStatus PlayerGetStatus(const Player *player);

Bullet *PlayerGetBullet(Player *player, const int bullet_index);
int PlayerGetBulletCount(const Player *player);
bool PlayerRemoveBullet(Player *player, const int bullet_index);
bool PlayerRemoveBullets(Player *player, int *bullet_indexes, const int bullet_index_count);

void PlayerIncrementKillCount(Player *player);
int PlayerGetKillCount(const Player *player);

void PlayerTakeDamage(Player *player);

void PlayerUpdate(Player *player, const ControllerInput input, const float delta);
void PlayerDraw(const Player *player);
