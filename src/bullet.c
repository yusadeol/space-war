#include "bullet.h"

#include "animation.h"
#include "asset.h"
#include "geometry.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Bullet {
    Texture2D texture;
    BulletDirection direction;
    Vector2 position;
    Animation *animation;
};

static Texture2D GetTexture(const BulletType type) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return *AssetGetTexture(TEXTURE_BULLET_PULSE);
    case BULLET_TYPE_BOLT:
        return *AssetGetTexture(TEXTURE_BULLET_BOLT);
    case BULLET_TYPE_HAMMER:
        return *AssetGetTexture(TEXTURE_BULLET_HAMMER);
    }

    return *AssetGetTexture(TEXTURE_BULLET_PULSE);
}

static Frame *GetFrames(const BulletType type, int *frame_count) {
    switch (type) {
    case BULLET_TYPE_PULSE:
        return AssetGetFrames(TEXTURE_BULLET_PULSE, frame_count);
    case BULLET_TYPE_BOLT:
        return AssetGetFrames(TEXTURE_BULLET_BOLT, frame_count);
    case BULLET_TYPE_HAMMER:
        return AssetGetFrames(TEXTURE_BULLET_HAMMER, frame_count);
    }

    return AssetGetFrames(TEXTURE_BULLET_PULSE, frame_count);
}

Bullet *BulletCreate(const BulletType type, const BulletDirection direction, const Rectangle shooter_bounds) {
    int frame_count;
    Frame *frames = GetFrames(type, &frame_count);
    Animation *animation = AnimationCreate(frames, frame_count);
    if (animation == NULL) {
        return NULL;
    }

    Bullet *bullet = malloc(sizeof(*bullet));

    if (bullet == NULL) {
        AnimationDestroy(animation);

        return NULL;
    }

    Vector2 shooter_center = GeometryGetCenterFromRectangle(shooter_bounds);

    *bullet = (Bullet){.texture = GetTexture(type), .direction = direction, .animation = animation};

    bullet->position = GeometryGetCenteredPosition(shooter_center, BulletGetWidth(bullet), BulletGetHeight(bullet));

    return bullet;
}

void BulletDestroy(Bullet *bullet) {
    assert(bullet);

    AnimationDestroy(bullet->animation);
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

void BulletUpdate(Bullet *bullet, const float delta) {
    assert(bullet);

    Vector2 previous_frame_center = GeometryGetCenterFromRectangle(BulletGetBounds(bullet));

    AnimationUpdate(bullet->animation, delta);

    float move_step = BULLET_SPEED * delta;
    Vector2 centered_position =
        GeometryGetCenteredPosition(previous_frame_center, BulletGetWidth(bullet), BulletGetHeight(bullet));

    bullet->position = centered_position;
    bullet->position.x += move_step * bullet->direction;
}

void BulletDraw(const Bullet *bullet) {
    assert(bullet);

    Frame frame = AnimationGetCurrentFrame(bullet->animation);

    Rectangle source = {.x = frame.x, .y = frame.y, .width = frame.width * bullet->direction, .height = frame.height};
    Rectangle destination = {bullet->position.x, bullet->position.y, BulletGetWidth(bullet), BulletGetHeight(bullet)};

    DrawTexturePro(bullet->texture, source, destination, (Vector2){}, 0, WHITE);
}
