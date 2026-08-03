#pragma once
#include "bullet.h"
#include <raylib.h>

constexpr float ENEMY_ATTACK_DISTANCE = 40.0f;
constexpr float ENEMY_SHOT_COOLDOWN = 0.5f;
constexpr int MAX_ENEMIES = 10;
constexpr float ENEMY_SPEED = 100.0f;
constexpr float ENEMY_SCALE = 1.0f;

typedef enum { ENEMY_TYPE_SPECTRA } EnemyType;

typedef struct Enemy Enemy;

Enemy *EnemyCreate(
    const EnemyType type, const int window_width, const int world_height,
    const int world_border);
void EnemyDestroy(Enemy *enemy);

float EnemyGetWidth(const Enemy *enemy);
float EnemyGetHeight(const Enemy *enemy);
Vector2 *EnemyGetPosition(Enemy *enemy);
Rectangle EnemyGetBounds(const Enemy *enemy);
Vector2 EnemyGetCenterPosition(const Enemy *enemy);
Bullet *EnemyGetBullet(Enemy *enemy, const int index);
int EnemyGetBulletCount(const Enemy *enemy);

void EnemyUpdate(
    Enemy *enemy, const int world_width, const Vector2 player_position,
    const Vector2 player_center_position, const float delta);
void EnemyDraw(const Enemy *enemy);
bool EnemyShoot(Enemy *enemy);
void EnemyRemoveBullet(Enemy *enemy, const int index);
