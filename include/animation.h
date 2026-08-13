#pragma once

constexpr float ANIMATION_FRAME_TIMER = 0.1f;

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Frame;

typedef struct Animation Animation;

Animation *AnimationCreate(Frame *frames, const int frame_count);
void AnimationDestroy(Animation *animation);

void AnimationUpdate(Animation *animation, const float delta);
Frame AnimationGetCurrentFrame(const Animation *animation);
