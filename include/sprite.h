#pragma once

#include <raylib.h>

typedef struct {
    float x;
    float y;
    float width;
    float height;
} Frame;

typedef struct {
    Frame* frames;
    int frame_count;
    float width;
    float height;
} Metadata;

typedef struct Sprite Sprite;

Sprite* SpriteCreate(Texture2D texture, Metadata metadata);
void SpriteDestroy(Sprite* sprite);

Texture2D SpriteGetTexture(const Sprite* sprite);

Frame SpriteGetFrame(const Sprite* sprite, const int frame_index);
int SpriteGetFrameCount(const Sprite* sprite);
