#include "texture.h"
#include <raylib.h>

static Texture2D textures[TEXTURE_COUNT];
static const char *file[TEXTURE_COUNT] = {
    [TEXTURE_SPACESHIP_VIPER] = "assets/textures/spaceships/viper.png",
    [TEXTURE_BULLET_PULSE] = "assets/textures/bullets/pulse.png",
};

void TextureLoad(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        textures[i] = LoadTexture(file[i]);
    }
}

void TextureUnload(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        UnloadTexture(textures[i]);
    }
}

Texture2D *TextureGet(TextureType type) {
    return &textures[type];
}
