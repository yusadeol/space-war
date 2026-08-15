#pragma once

#include "sprite.h"

#include <raylib.h>

typedef enum {
    TEXTURE_SPACESHIP_VIPER,
    TEXTURE_SPACESHIP_SPECTRA,
    TEXTURE_SPACESHIP_RAPTOR,

    TEXTURE_BULLET_PULSE,
    TEXTURE_BULLET_BOLT,
    TEXTURE_BULLET_HAMMER,

    TEXTURE_COUNT
} TextureType;

void AssetLoadTextures(void);
void AssetUnloadTextures(void);
Texture2D AssetGetTexture(const TextureType type);

void AssetLoadMetadatas(void);
void AssetUnloadMetadatas(void);
Metadata AssetGetMetadata(const TextureType type);

Sprite *AssetGetSprite(const TextureType type);
