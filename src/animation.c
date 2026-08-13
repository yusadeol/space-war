#include "animation.h"

#include <assert.h>
#include <stdlib.h>

struct Animation {
    Frame *frames;
    int frame_count;
    int current_frame;
    float timer;
};

Animation *AnimationCreate(Frame *frames, const int frame_count) {
    if (frames == NULL) {
        return NULL;
    }

    if (frame_count <= 0) {
        free(frames);

        return NULL;
    }

    Animation *animation = malloc(sizeof(*animation));
    if (animation == NULL) {
        free(frames);

        return NULL;
    }

    *animation = (Animation){
        .frames = frames,
        .frame_count = frame_count,
        .timer = ANIMATION_FRAME_TIMER,
    };

    return animation;
}

void AnimationDestroy(Animation *animation) {
    assert(animation);

    free(animation->frames);
    free(animation);
}

void AnimationUpdate(Animation *animation, const float delta) {
    assert(animation);

    animation->timer -= delta;

    if (animation->timer <= 0.0f) {
        animation->timer = ANIMATION_FRAME_TIMER;

        if (animation->current_frame >= (animation->frame_count - 1)) {
            return;
        }

        animation->current_frame++;
    }
}

Frame AnimationGetCurrentFrame(const Animation *animation) {
    assert(animation);

    return animation->frames[animation->current_frame];
}
