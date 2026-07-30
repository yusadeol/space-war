#include "bullet.h"
#include "asset.h"
#include <raylib.h>

Texture2D BulletTexture(BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return *AssetGetTexture(TEXTURE_BULLET_PULSE);
    }

    return (Texture2D){};
}

Bullet BulletCreate(BulletType type, Vector2 position) {
    return (Bullet){.texture = BulletTexture(type), .position = position};
}

void BulletUpdate(Bullet *bullet, float delta) {
    float move_step = BULLET_SPEED * delta;

    bullet->position.x += move_step;
}

float BulletGetWidth(const Bullet *bullet) {
    return bullet->texture.width * BULLET_SCALE;
}

float BulletGetHeight(const Bullet *bullet) {
    return bullet->texture.height * BULLET_SCALE;
}

void BulletDraw(const Bullet *bullet) {
    Rectangle source = {.width = bullet->texture.width,
                        .height = bullet->texture.height};
    Rectangle destination = {bullet->position.x, bullet->position.y,
                             BulletGetWidth(bullet), BulletGetHeight(bullet)};

    DrawTexturePro(bullet->texture, source, destination, (Vector2){}, 0, WHITE);
}
