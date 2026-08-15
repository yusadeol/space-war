#pragma once

#include "sprite.h"

typedef struct Animation Animation;

Animation* AnimationCreate(const Sprite* sprite, const float frame_duration);
void AnimationDestroy(Animation* animation);

void AnimationSetFrame(Animation* animation, const int frame_index);
Frame AnimationGetCurrentFrame(const Animation* animation);

void AnimationUpdate(Animation* animation, const float delta);
void AnimationReset(Animation* animation);

bool AnimationIsFinished(const Animation* animation);
