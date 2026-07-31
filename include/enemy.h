#ifndef ENEMY_H
#define ENEMY_H

#include "asset.h"
#include "bullet.h"
#include <raylib.h>

constexpr int MAX_ENEMIES = 2;
constexpr float ENEMY_SPEED = 400.0f;
constexpr float ENEMY_SCALE = 1.0f;

typedef enum { ENEMY_TYPE_SPECTRA } EnemyType;

typedef struct Enemy Enemy;

Enemy *EnemyCreate(const EnemyType type);
void EnemyDestroy(Enemy *enemy);

float EnemyGetWidth(const Enemy *enemy);
float EnemyGetHeight(const Enemy *enemy);
Vector2 *EnemyGetPosition(Enemy *enemy);
TextureSize EnemyGetSize(const Enemy *enemy);
Bullet *EnemyGetBullet(Enemy *enemy, const int index);
int EnemyGetBulletCount(const Enemy *enemy);

void EnemyUpdate(Enemy *enemy, float delta);
void EnemyDraw(const Enemy *enemy);
bool EnemyShot(Enemy *enemy);
void EnemySpliceBullet(Enemy *enemy, const int index);

#endif
