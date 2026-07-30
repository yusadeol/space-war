#ifndef ASSET_H
#define ASSET_H

#include "raylib.h"

typedef enum {
    TEXTURE_SPACESHIP_VIPER,
    TEXTURE_BULLET_PULSE,

    TEXTURE_COUNT
} TextureType;

void AssetLoadTextures(void);
void AssetUnloadTextures(void);

Texture2D *AssetGetTexture(TextureType type);

#endif
