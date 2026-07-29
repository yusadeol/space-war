#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

typedef enum {
    TEXTURE_SPACESHIP_VIPER,
    TEXTURE_BULLET_PULSE,

    TEXTURE_COUNT
} TextureType;

void TextureLoad(void);
void TextureUnload(void);

Texture2D *TextureGet(TextureType type);

#endif
