#ifndef PLAYER_H
#define PLAYER_H

#include "asset.h"
#include "bullet.h"

constexpr int MAX_PLAYERS = 2;
constexpr float PLAYER_SPEED = 400.0f;
constexpr float PLAYER_SCALE = 1.0f;

typedef enum { SPACESHIP_TYPE_VIPER } SpaceshipType;

typedef struct Player Player;

Player *PlayerCreate(SpaceshipType type);
void PlayerDestroy(Player *player);

float PlayerGetWidth(const Player *player);
float PlayerGetHeight(const Player *player);
Vector2 *PlayerGetPosition(Player *player);
TextureSize PlayerGetSize(const Player *player);
Bullet *PlayerGetBullet(Player *player, int index);
int PlayerGetBulletCount(const Player *player);

void PlayerUpdate(Player *player, float delta);
void PlayerDraw(const Player *player);
void PlayerShot(Player *player);
void PlayerSpliceBullet(Player *player, int index);

#endif
