#ifndef ASSET_H
#define ASSET_H

#include <raylib.h>

typedef enum {
    TEXTURE_SPACESHIP_VIPER,
    TEXTURE_SPACESHIP_SPECTRA,
    TEXTURE_BULLET_PULSE,

    TEXTURE_COUNT
} TextureType;

typedef struct TextureSize {
    int width;
    int height;
} TextureSize;

void AssetLoadTextures(void);
void AssetUnloadTextures(void);

Texture2D *AssetGetTexture(TextureType type);

#endif
