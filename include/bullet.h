#ifndef BULLET_H
#define BULLET_H

#include "raylib.h"

constexpr int MAX_SIMULTANEOUS_BULLETS = 50;
constexpr float BULLET_SPEED = 800.0f;
constexpr float BULLET_SCALE = 1.0f;

typedef enum { BULLET_TYPE_PULSE } BulletType;

typedef struct Bullet {
    Texture2D texture;
    Vector2 position;
} Bullet;

Texture2D BulletTexture(BulletType type);
Bullet BulletCreate(BulletType type, Vector2 position);
void BulletUpdate(Bullet *bullet, float delta);
float BulletWidth(const Bullet *bullet);
float BulletHeight(const Bullet *bullet);
void BulletDraw(const Bullet *bullet);

#endif
