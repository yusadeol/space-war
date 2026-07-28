#include "bullet.h"
#include <raylib.h>

Texture2D BulletTexture(BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return LoadTexture("assets/textures/bullets/pulse.png");
    }

    return (Texture2D){};
}

Bullet BulletCreate(BulletType type, Vector2 position) {
    return (Bullet){ .texture = BulletTexture(type), .position = position };
}

void BulletUpdate(Bullet *bullet, float delta) {
    float move_step = BULLET_SPEED * delta;

    bullet->position.x += move_step;
}

float BulletWidth(const Bullet *bullet) { return bullet->texture.width * BULLET_SCALE; }

float BulletHeight(const Bullet *bullet) { return bullet->texture.height * BULLET_SCALE; }

void BulletDraw(const Bullet *bullet) {
    Rectangle source = { .width = bullet->texture.width, .height = bullet->texture.height };
    Rectangle destination = { bullet->position.x, bullet->position.y, BulletWidth(bullet), BulletHeight(bullet) };

    DrawTexturePro(bullet->texture, source, destination, (Vector2){}, 0, WHITE);
}
