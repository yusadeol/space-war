#pragma once

#include <raylib.h>

#include "sprite.h"

typedef enum {
    ASSET_SPACESHIP_VIPER,
    ASSET_SPACESHIP_SPECTRA,
    ASSET_SPACESHIP_RAPTOR,

    ASSET_BULLET_PULSE,
    ASSET_BULLET_BOLT,
    ASSET_BULLET_HAMMER,

    ASSET_COUNT
} AssetType;

void AssetLoadTextures(void);
void AssetUnloadTextures(void);
Texture2D AssetGetTexture(const AssetType type);

void AssetLoadMetadatas(void);
void AssetUnloadMetadatas(void);
Metadata AssetGetMetadata(const AssetType type);

Sprite* AssetGetSprite(const AssetType type);
