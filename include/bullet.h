#pragma once
#include "geometry.h"
#include <raylib.h>

constexpr int MAX_SIMULTANEOUS_BULLETS = 50;
constexpr float BULLET_SPEED = 800.0f;
constexpr float BULLET_SCALE = 1.0f;

typedef enum { BULLET_TYPE_PULSE } BulletType;
typedef enum {
    BULLET_DIRECTION_LEFT = -1,
    BULLET_DIRECTION_RIGHT = 1,
} BulletDirection;

typedef struct Bullet Bullet;

Bullet *BulletCreate(
    const BulletType type, const BulletDirection direction,
    const Vector2 position);
void BulletDestroy(Bullet *bullet);

float BulletGetWidth(const Bullet *bullet);
float BulletGetHeight(const Bullet *bullet);
Vector2 *BulletGetPosition(Bullet *bullet);
Size BulletGetSize(const Bullet *bullet);
Rectangle BulletGetBounds(const Bullet *bullet);

void BulletUpdate(Bullet *bullet, const float delta);
void BulletDraw(const Bullet *bullet);
