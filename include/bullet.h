#pragma once
#include <raylib.h>

constexpr int MAX_SIMULTANEOUS_BULLETS = 50;
constexpr float BULLET_SPEED = 800.0f;
constexpr float BULLET_SCALE = 1.0f;

typedef enum { BULLET_TYPE_PULSE, BULLET_TYPE_BOLT, BULLET_TYPE_HAMMER } BulletType;
typedef enum { BULLET_DIRECTION_RIGHT = 1, BULLET_DIRECTION_LEFT = -1 } BulletDirection;
typedef enum { BULLET_STATUS_NORMAL, BULLET_STATUS_DESTROYED } BulletStatus;

typedef struct Bullet Bullet;

Bullet *BulletCreate(const BulletType type, const BulletDirection direction, const Vector2 shooter_center_position);
void BulletDestroy(Bullet *bullet);

float BulletGetWidth(const Bullet *bullet);
float BulletGetHeight(const Bullet *bullet);

Vector2 BulletGetPosition(const Bullet *bullet);
Rectangle BulletGetBounds(const Bullet *bullet);
Vector2 BulletGetCenterPosition(const Bullet *bullet);

BulletStatus BulletGetStatus(const Bullet *bullet);

void BulletHit(Bullet *bullet);

void BulletUpdate(Bullet *bullet, const float delta);
void BulletDraw(const Bullet *bullet);
