#include "animation.h"

#include "sprite.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Animation {
    const Sprite *sprite;
    int current_frame;
    float frame_duration;
    float frame_countdown;
    bool is_finished;
};

Animation *AnimationCreate(const Sprite *sprite, const float frame_duration) {
    if (sprite == NULL) {
        return NULL;
    }

    Animation *animation = malloc(sizeof(*animation));
    if (animation == NULL) {
        return NULL;
    }

    *animation = (Animation){.sprite = sprite, .frame_duration = frame_duration, .frame_countdown = frame_duration};

    return animation;
}

void AnimationDestroy(Animation *animation) {
    assert(animation);

    free(animation);
}

void AnimationSetFrame(Animation *animation, const int frame_index) {
    assert(animation);

    if (frame_index < 0 || frame_index >= SpriteGetFrameCount(animation->sprite)) {
        TraceLog(LOG_ERROR, "Invalid frame index %d for sprite with %d frames", frame_index,
            SpriteGetFrameCount(animation->sprite));

        return;
    }

    animation->current_frame = frame_index;
}

Frame AnimationGetCurrentFrame(const Animation *animation) {
    assert(animation);

    return SpriteGetFrame(animation->sprite, animation->current_frame);
}

void AnimationUpdate(Animation *animation, const float delta) {
    assert(animation);

    animation->frame_countdown -= delta;

    if (animation->frame_countdown <= 0.0f) {
        animation->frame_countdown = animation->frame_duration;

        if (animation->current_frame >= (SpriteGetFrameCount(animation->sprite) - 1)) {
            animation->is_finished = true;

            return;
        }

        animation->current_frame++;
    }
}

void AnimationReset(Animation *animation) {
    assert(animation);

    animation->current_frame = 0;
    animation->frame_countdown = animation->frame_duration;
    animation->is_finished = false;
}

bool AnimationIsFinished(const Animation *animation) {
    assert(animation);

    return animation->is_finished;
}
