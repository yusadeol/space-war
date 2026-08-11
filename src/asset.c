#include "asset.h"

#include <raylib.h>
#include <stddef.h>

static Texture2D textures[TEXTURE_COUNT];
static const char *file_textures[TEXTURE_COUNT] = {
    [TEXTURE_SPACESHIP_VIPER] = "assets/textures/spaceships/viper.png",
    [TEXTURE_SPACESHIP_SPECTRA] = "assets/textures/spaceships/spectra.png",
    [TEXTURE_SPACESHIP_RAPTOR] = "assets/textures/spaceships/raptor.png",
    [TEXTURE_BULLET_PULSE] = "assets/textures/bullets/pulse.png",
    [TEXTURE_BULLET_BOLT] = "assets/textures/bullets/bolt.png",
    [TEXTURE_BULLET_HAMMER] = "assets/textures/bullets/hammer.png",
};

void AssetLoadTextures(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        textures[i] = LoadTexture(file_textures[i]);
    }
}

void AssetUnloadTextures(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        UnloadTexture(textures[i]);
    }
}

Texture2D *AssetGetTexture(const TextureType type) {
    if (type < 0 || type >= TEXTURE_COUNT) {
        return NULL;
    }

    return &textures[type];
}
