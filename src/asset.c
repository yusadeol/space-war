#include "asset.h"

#include <raylib.h>
#include <stdlib.h>
#include <yyjson.h>

static Texture2D textures[TEXTURE_COUNT];

static const char *file_textures[TEXTURE_COUNT] = {
    [TEXTURE_SPACESHIP_VIPER] = "assets/textures/spaceships/viper.png",
    [TEXTURE_SPACESHIP_SPECTRA] = "assets/textures/spaceships/spectra.png",
    [TEXTURE_SPACESHIP_RAPTOR] = "assets/textures/spaceships/raptor.png",
    [TEXTURE_BULLET_PULSE] = "assets/textures/bullets/pulse.png",
    [TEXTURE_BULLET_BOLT] = "assets/textures/bullets/bolt.png",
    [TEXTURE_BULLET_HAMMER] = "assets/textures/bullets/hammer.png",
};

static const char *file_frames[TEXTURE_COUNT] = {
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

Frame *AssetGetFrames(const TextureType type, int *frame_count) {
    if (type < 0 || type >= TEXTURE_COUNT || file_frames[type] == NULL) {
        TraceLog(LOG_ERROR, "No frames file registered for texture type %d", type);

        return NULL;
    }

    yyjson_read_err error;

    yyjson_doc *json = yyjson_read_file(file_frames[type], 0, NULL, &error);
    if (json == NULL) {
        TraceLog(LOG_ERROR, "Failed to read frames file %s: %s", file_frames[type], error.msg);

        return NULL;
    }

    yyjson_val *json_root = yyjson_doc_get_root(json);
    if (json_root == NULL) {
        TraceLog(LOG_ERROR, "Failed to parse frames file %s", file_frames[type]);
        yyjson_doc_free(json);

        return NULL;
    }

    yyjson_val *json_frames = yyjson_obj_get(json_root, "frames");
    if (json_frames == NULL) {
        TraceLog(LOG_ERROR, "Frames file %s is missing the frames key", file_frames[type]);
        yyjson_doc_free(json);

        return NULL;
    }

    size_t json_frame_count = yyjson_arr_size(json_frames);
    if (json_frame_count <= 0) {
        TraceLog(LOG_ERROR, "Frames file %s contains no frames", file_frames[type]);
        yyjson_doc_free(json);

        return NULL;
    }

    Frame *frames = malloc(sizeof(*frames) * json_frame_count);

    if (frames == NULL) {
        TraceLog(LOG_ERROR, "Failed to allocate frames array for texture type %d", type);
        yyjson_doc_free(json);

        return NULL;
    }

    for (size_t i = 0; i < json_frame_count; i++) {
        yyjson_val *frame = yyjson_arr_get(json_frames, i);

        yyjson_val *x = yyjson_obj_get(frame, "x");
        yyjson_val *y = yyjson_obj_get(frame, "y");
        yyjson_val *width = yyjson_obj_get(frame, "width");
        yyjson_val *height = yyjson_obj_get(frame, "height");

        if (x == NULL || y == NULL || width == NULL || height == NULL) {
            TraceLog(LOG_ERROR, "Frames file %s has a frame with missing fields", file_frames[type]);
            free(frames);
            yyjson_doc_free(json);

            return NULL;
        }

        frames[i] = (Frame){
            .x = yyjson_get_num(x),
            .y = yyjson_get_num(y),
            .width = yyjson_get_num(width),
            .height = yyjson_get_num(height),
        };
    }

    yyjson_doc_free(json);

    *frame_count = (int)json_frame_count;
    return frames;
}
