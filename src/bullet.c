#include "bullet.h"
#include "asset.h"
#include <raylib.h>
#include <stdlib.h>

struct Bullet {
    Texture2D texture;
    BulletDirection direction;
    Vector2 position;
};

static Texture2D BulletTexture(const BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return *AssetGetTexture(TEXTURE_BULLET_PULSE);
    }

    return (Texture2D){};
}

Bullet *BulletCreate(
    const BulletType type, const BulletDirection direction,
    const Vector2 position) {
    Bullet *bullet = malloc(sizeof(*bullet));

    if (bullet == NULL) {
        return NULL;
    }

    *bullet = (Bullet){.texture = BulletTexture(type),
                       .direction = direction,
                       .position = position};

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

Size BulletGetSize(const Bullet *bullet) {
    return (Size){BulletGetWidth(bullet), BulletGetHeight(bullet)};
}

Rectangle BulletGetBounds(const Bullet *bullet) {
    return (Rectangle){
        .x = bullet->position.x,
        .y = bullet->position.y,
        .width = BulletGetWidth(bullet),
        .height = BulletGetHeight(bullet),
    };
}

void BulletUpdate(Bullet *bullet, const float delta) {
    float move_step = BULLET_SPEED * delta;

    bullet->position.x += move_step * bullet->direction;
}

void BulletDraw(const Bullet *bullet) {
    Rectangle source = {.width = bullet->texture.width * bullet->direction,
                        .height = bullet->texture.height};
    Rectangle destination = {bullet->position.x, bullet->position.y,
                             BulletGetWidth(bullet), BulletGetHeight(bullet)};

    DrawTexturePro(bullet->texture, source, destination, (Vector2){}, 0, WHITE);
}
