#pragma once
#include <raylib.h>

typedef enum {
    TEXTURE_SPACESHIP_VIPER,
    TEXTURE_SPACESHIP_SPECTRA,
    TEXTURE_BULLET_PULSE,

    TEXTURE_COUNT
} TextureType;

void AssetLoadTextures(void);
void AssetUnloadTextures(void);

Texture2D *AssetGetTexture(const TextureType type);
