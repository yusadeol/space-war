#include "sprite.h"

#include <assert.h>
#include <stdlib.h>

struct Sprite {
    Texture2D texture;
    Metadata metadata;
};

Sprite* SpriteCreate(Texture2D texture, Metadata metadata) {
    Sprite* sprite = malloc(sizeof(*sprite));
    if (sprite == NULL) {
        return NULL;
    }

    *sprite = (Sprite){.texture = texture, .metadata = metadata};

    return sprite;
}

void SpriteDestroy(Sprite* sprite) {
    assert(sprite);

    free(sprite);
}

Texture2D SpriteGetTexture(const Sprite* sprite) {
    assert(sprite);

    return sprite->texture;
}

Frame SpriteGetFrame(const Sprite* sprite, const int frame_index) {
    assert(sprite);

    if (frame_index < 0 || frame_index >= sprite->metadata.frame_count) {
        return (Frame){};
    }

    return sprite->metadata.frames[frame_index];
}

int SpriteGetFrameCount(const Sprite* sprite) {
    assert(sprite);

    return sprite->metadata.frame_count;
}
