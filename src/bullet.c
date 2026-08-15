#include "bullet.h"

#include "animation.h"
#include "asset.h"
#include "geometry.h"
#include "sprite.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static constexpr float FRAME_DURATION = 0.1f;

struct Bullet {
    Sprite *sprite;
    Animation *animation;
    BulletDirection direction;
    Vector2 position;
    BulletStatus status;
};

static Sprite *GetSprite(const BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return AssetGetSprite(TEXTURE_BULLET_PULSE);
    case BULLET_TYPE_BOLT:
        return AssetGetSprite(TEXTURE_BULLET_BOLT);
    case BULLET_TYPE_HAMMER:
        return AssetGetSprite(TEXTURE_BULLET_HAMMER);
    }

    return AssetGetSprite(TEXTURE_BULLET_PULSE);
}

Bullet *BulletCreate(const BulletType type, const BulletDirection direction, const Vector2 shooter_center_position) {
    Sprite *sprite = GetSprite(type);
    if (sprite == NULL) {
        return NULL;
    }

    Animation *animation = AnimationCreate(sprite, FRAME_DURATION);
    if (animation == NULL) {
        SpriteDestroy(sprite);

        return NULL;
    }

    Bullet *bullet = malloc(sizeof(*bullet));
    if (bullet == NULL) {
        AnimationDestroy(animation);
        SpriteDestroy(sprite);

        return NULL;
    }

    *bullet = (Bullet){
        .sprite = sprite,
        .animation = animation,
        .direction = direction,
        .status = BULLET_STATUS_NORMAL,
    };

    bullet->position =
        GeometryGetCenteredPosition(shooter_center_position, BulletGetWidth(bullet), BulletGetHeight(bullet));

    return bullet;
}

void BulletDestroy(Bullet *bullet) {
    assert(bullet);

    AnimationDestroy(bullet->animation);
    SpriteDestroy(bullet->sprite);
    free(bullet);
}

float BulletGetWidth(const Bullet *bullet) {
    assert(bullet);

    Frame frame = AnimationGetCurrentFrame(bullet->animation);

    return frame.width * BULLET_SCALE;
}

float BulletGetHeight(const Bullet *bullet) {
    assert(bullet);

    Frame frame = AnimationGetCurrentFrame(bullet->animation);

    return frame.height * BULLET_SCALE;
}

Vector2 BulletGetPosition(const Bullet *bullet) {
    assert(bullet);

    return bullet->position;
}

Rectangle BulletGetBounds(const Bullet *bullet) {
    assert(bullet);

    return (Rectangle){
        .x = bullet->position.x,
        .y = bullet->position.y,
        .width = BulletGetWidth(bullet),
        .height = BulletGetHeight(bullet),
    };
}

Vector2 BulletGetCenterPosition(const Bullet *bullet) {
    assert(bullet);

    return GeometryGetCenterFromRectangle(BulletGetBounds(bullet));
}

BulletStatus BulletGetStatus(const Bullet *bullet) {
    assert(bullet);

    return bullet->status;
}

void BulletHit(Bullet *bullet) {
    assert(bullet);

    if (bullet->status == BULLET_STATUS_DESTROYED) {
        return;
    }

    bullet->status = BULLET_STATUS_DESTROYED;
}

void BulletUpdate(Bullet *bullet, const float delta) {
    assert(bullet);

    Vector2 previous_center_position = BulletGetCenterPosition(bullet);

    AnimationUpdate(bullet->animation, delta);

    float move_step = BULLET_SPEED * delta;
    Vector2 centered_position =
        GeometryGetCenteredPosition(previous_center_position, BulletGetWidth(bullet), BulletGetHeight(bullet));

    bullet->position = centered_position;
    bullet->position.x += move_step * bullet->direction;
}

void BulletDraw(const Bullet *bullet) {
    assert(bullet);

    Frame frame = AnimationGetCurrentFrame(bullet->animation);

    Rectangle source = {.x = frame.x, .y = frame.y, .width = frame.width * bullet->direction, .height = frame.height};
    Rectangle destination = {bullet->position.x, bullet->position.y, BulletGetWidth(bullet), BulletGetHeight(bullet)};

    DrawTexturePro(SpriteGetTexture(bullet->sprite), source, destination, (Vector2){}, 0, WHITE);
}
