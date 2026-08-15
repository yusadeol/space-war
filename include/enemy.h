#pragma once
#include <raylib.h>

#include "bullet.h"

constexpr float ENEMY_BEHAVIOR_COOLDOWN = 5.0f;
constexpr float ENEMY_SHOT_COOLDOWN = 0.5f;
constexpr float ENEMY_ATTACK_DISTANCE = 40.0f;

constexpr int MAX_ENEMIES = 10;
constexpr float ENEMY_SPEED = 100.0f;
constexpr float ENEMY_SCALE = 1.0f;

typedef enum { ENEMY_TYPE_SPECTRA } EnemyType;
typedef enum { ENEMY_DIRECTION_RIGHT = 1, ENEMY_DIRECTION_LEFT = -1 } EnemyDirection;
typedef enum { ENEMY_STATUS_NORMAL, ENEMY_STATUS_DAMAGED, ENEMY_STATUS_DESTROYED, ENEMY_STATUS_EXPLODED } EnemyStatus;
typedef enum { ENEMY_BEHAVIOR_PURSUIT, ENEMY_BEHAVIOR_RETREAT } EnemyBehavior;

typedef struct Enemy Enemy;

Enemy* EnemyCreate(const EnemyType type, const int window_width, const int world_height, const int world_border);
void EnemyDestroy(Enemy* enemy);

float EnemyGetWidth(const Enemy* enemy);
float EnemyGetHeight(const Enemy* enemy);

Vector2 EnemyGetPosition(const Enemy* enemy);
Rectangle EnemyGetBounds(const Enemy* enemy);
Vector2 EnemyGetCenterPosition(const Enemy* enemy);

EnemyStatus EnemyGetStatus(const Enemy* enemy);

Bullet* EnemyGetBullet(Enemy* enemy, const int bullet_index);
int EnemyGetBulletCount(const Enemy* enemy);
bool EnemyRemoveBullet(Enemy* enemy, const int bullet_index);
bool EnemyRemoveBullets(Enemy* enemy, int* bullet_indexes, const int bullet_index_count);

void EnemyTakeDamage(Enemy* enemy);

void EnemyUpdate(Enemy* enemy, const int window_height, const int world_width, const Vector2 player_position,
                 const Vector2 player_center_position, const float delta);
void EnemyDraw(const Enemy* enemy);
