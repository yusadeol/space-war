#include "asset.h"

#include "sprite.h"

#include <raylib.h>
#include <stdlib.h>
#include <yyjson.h>

static Texture2D *textures[TEXTURE_COUNT];

static const char *file_textures[TEXTURE_COUNT] = {
    [TEXTURE_SPACESHIP_VIPER] = "assets/textures/spaceships/viper.png",
    [TEXTURE_SPACESHIP_SPECTRA] = "assets/textures/spaceships/spectra.png",
    [TEXTURE_SPACESHIP_RAPTOR] = "assets/textures/spaceships/raptor.png",

    [TEXTURE_BULLET_PULSE] = "assets/textures/bullets/pulse.png",
    [TEXTURE_BULLET_BOLT] = "assets/textures/bullets/bolt.png",
    [TEXTURE_BULLET_HAMMER] = "assets/textures/bullets/hammer.png",
};

static Metadata *metadatas[TEXTURE_COUNT];

static const char *file_metadatas[TEXTURE_COUNT] = {
    [TEXTURE_SPACESHIP_VIPER] = "assets/textures/spaceships/viper.json",
    [TEXTURE_SPACESHIP_SPECTRA] = "assets/textures/spaceships/spectra.json",
    [TEXTURE_SPACESHIP_RAPTOR] = "assets/textures/spaceships/raptor.json",

    [TEXTURE_BULLET_PULSE] = "assets/textures/bullets/pulse.json",
    [TEXTURE_BULLET_BOLT] = "assets/textures/bullets/bolt.json",
    [TEXTURE_BULLET_HAMMER] = "assets/textures/bullets/hammer.json",
};

void AssetLoadTextures(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        if (file_textures[i] == NULL) {
            TraceLog(LOG_ERROR, "No texture file registered for texture type %d", i);

            continue;
        }

        Texture2D *texture = malloc(sizeof(*texture));
        if (texture == NULL) {
            TraceLog(LOG_ERROR, "Failed to allocate texture for texture type %d", i);

            continue;
        }

        *texture = LoadTexture(file_textures[i]);
        textures[i] = texture;
    }
}

void AssetUnloadTextures(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        Texture2D *texture = textures[i];
        if (texture == NULL) {
            continue;
        }

        UnloadTexture(*texture);
        free(texture);
        textures[i] = NULL;
    }
}

Texture2D AssetGetTexture(const TextureType type) {
    if (type < 0 || type >= TEXTURE_COUNT) {
        return (Texture2D){};
    }

    Texture2D *texture = textures[type];
    if (texture == NULL) {
        return (Texture2D){};
    }

    return *textures[type];
}

void AssetLoadMetadatas(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        if (file_metadatas[i] == NULL) {
            TraceLog(LOG_ERROR, "No metadata file registered for texture type %d", i);

            continue;
        }

        yyjson_read_err error;

        yyjson_doc *json = yyjson_read_file(file_metadatas[i], 0, NULL, &error);
        if (json == NULL) {
            TraceLog(LOG_ERROR, "Failed to read metadata file %s: %s", file_metadatas[i], error.msg);

            continue;
        }

        yyjson_val *json_root = yyjson_doc_get_root(json);
        if (json_root == NULL) {
            TraceLog(LOG_ERROR, "Failed to parse metadata file %s", file_metadatas[i]);
            yyjson_doc_free(json);

            continue;
        }

        Metadata *metadata = malloc(sizeof(*metadata));
        if (metadata == NULL) {
            TraceLog(LOG_ERROR, "Failed to allocate metadata for texture type %d", i);
            yyjson_doc_free(json);

            continue;
        }

        yyjson_val *json_meta = yyjson_obj_get(json_root, "meta");
        if (json_meta == NULL) {
            TraceLog(LOG_ERROR, "Metadata file %s is missing the meta key", file_metadatas[i]);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        yyjson_val *json_size = yyjson_obj_get(json_meta, "size");
        if (json_size == NULL) {
            TraceLog(LOG_ERROR, "Metadata file %s is missing the size key", file_metadatas[i]);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        yyjson_val *width = yyjson_obj_get(json_size, "width");
        yyjson_val *height = yyjson_obj_get(json_size, "height");

        if (width == NULL || height == NULL) {
            TraceLog(LOG_ERROR, "Metadata file %s is missing the width or height key", file_metadatas[i]);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        *metadata = (Metadata){
            .width = yyjson_get_num(width),
            .height = yyjson_get_num(height),
        };

        yyjson_val *json_frames = yyjson_obj_get(json_root, "frames");
        if (json_frames == NULL) {
            TraceLog(LOG_ERROR, "Metadata file %s is missing the frames key", file_metadatas[i]);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        int json_frame_count = (int)yyjson_arr_size(json_frames);
        if (json_frame_count <= 0) {
            TraceLog(LOG_ERROR, "Metadata file %s contains no frames", file_metadatas[i]);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        Frame *frames = malloc(sizeof(*frames) * json_frame_count);
        if (frames == NULL) {
            TraceLog(LOG_ERROR, "Failed to allocate frames array for texture type %d", i);
            yyjson_doc_free(json);
            free(metadata);

            continue;
        }

        bool has_invalid_frame = false;

        for (int j = 0; j < json_frame_count; j++) {
            yyjson_val *frame = yyjson_arr_get(json_frames, j);

            yyjson_val *x = yyjson_obj_get(frame, "x");
            yyjson_val *y = yyjson_obj_get(frame, "y");
            yyjson_val *width = yyjson_obj_get(frame, "width");
            yyjson_val *height = yyjson_obj_get(frame, "height");

            if (x == NULL || y == NULL || width == NULL || height == NULL) {
                TraceLog(LOG_ERROR, "Metadata file %s has a frame with missing fields", file_metadatas[i]);

                has_invalid_frame = true;

                break;
            }

            frames[j] = (Frame){
                .x = yyjson_get_num(x),
                .y = yyjson_get_num(y),
                .width = yyjson_get_num(width),
                .height = yyjson_get_num(height),
            };
        }

        yyjson_doc_free(json);

        if (has_invalid_frame) {
            free(metadata);
            free(frames);

            continue;
        }

        metadata->frames = frames;
        metadata->frame_count = json_frame_count;

        metadatas[i] = metadata;
    }
}

void AssetUnloadMetadatas(void) {
    for (int i = 0; i < TEXTURE_COUNT; i++) {
        Metadata *metadata = metadatas[i];
        if (metadata == NULL) {
            continue;
        }

        free(metadata->frames);
        free(metadata);
        metadatas[i] = NULL;
    }
}

Metadata AssetGetMetadata(const TextureType type) {
    if (type < 0 || type >= TEXTURE_COUNT) {
        return (Metadata){};
    }

    Metadata *metadata = metadatas[type];
    if (metadata == NULL) {
        return (Metadata){};
    }

    return *metadatas[type];
}

Sprite *AssetGetSprite(const TextureType type) {
    Texture2D texture = AssetGetTexture(type);
    if (texture.id == 0) {
        return NULL;
    }

    Metadata metadata = AssetGetMetadata(type);
    if (metadata.frames == NULL || metadata.frame_count == 0 || metadata.width == 0 || metadata.height == 0) {
        return NULL;
    }

    return SpriteCreate(texture, metadata);
}
