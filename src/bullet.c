#include "bullet.h"
#include "asset.h"
#include <raylib.h>
#include <stdlib.h>

struct Bullet {
    Texture2D texture;
    Vector2 position;
};

static Texture2D BulletTexture(const BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return *AssetGetTexture(TEXTURE_BULLET_PULSE);
    }

    return (Texture2D){};
}

Bullet *BulletCreate(const BulletType type, const Vector2 position) {
    Bullet *bullet = malloc(sizeof(*bullet));

    if (bullet == NULL) {
        return NULL;
    }

    *bullet = (Bullet){.texture = BulletTexture(type), .position = position};

    return bullet;
}

void BulletDestroy(Bullet *bullet) {
    free(bullet);
}

float BulletGetWidth(const Bullet *bullet) {
    return bullet->texture.width * BULLET_SCALE;
}

float BulletGetHeight(const Bullet *bullet) {
    return bullet->texture.height * BULLET_SCALE;
}

Vector2 *BulletGetPosition(Bullet *bullet) {
    return &bullet->position;
}

TextureSize BulletGetSize(const Bullet *bullet) {
    return (TextureSize){bullet->texture.width, bullet->texture.height};
}

void BulletUpdate(Bullet *bullet, const float delta) {
    float move_step = BULLET_SPEED * delta;

    bullet->position.x += move_step;
}

void BulletDraw(const Bullet *bullet) {
    Rectangle source = {.width = bullet->texture.width,
                        .height = bullet->texture.height};
    Rectangle destination = {bullet->position.x, bullet->position.y,
                             BulletGetWidth(bullet), BulletGetHeight(bullet)};

    DrawTexturePro(bullet->texture, source, destination, (Vector2){}, 0, WHITE);
}
