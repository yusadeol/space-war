#ifndef BULLET_H
#define BULLET_H

#include "asset.h"
#include "raylib.h"

constexpr int MAX_SIMULTANEOUS_BULLETS = 50;
constexpr float BULLET_SPEED = 800.0f;
constexpr float BULLET_SCALE = 1.0f;

typedef enum { BULLET_TYPE_PULSE } BulletType;

typedef struct Bullet {
    Texture2D texture;
    Vector2 position;
} Bullet;

Bullet BulletCreate(BulletType type, Vector2 position);

float BulletGetWidth(const Bullet *bullet);
float BulletGetHeight(const Bullet *bullet);
TextureSize BulletGetSize(const Bullet *bullet);

void BulletUpdate(Bullet *bullet, float delta);
void BulletDraw(const Bullet *bullet);

#endif
