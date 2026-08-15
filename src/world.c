#include "world.h"

#include <assert.h>
#include <stdlib.h>

struct World {
    int width;
    int height;
    int border;
    Color background_color;
};

World *WorldCreate(const int window_width, const int window_height, const int border, Color background_color) {
    World *world = malloc(sizeof(*world));

    if (world == NULL) {
        return NULL;
    }

    *world = (World){
        .width = window_width - border,
        .height = window_height - border,
        .border = border,
        .background_color = background_color,
    };

    return world;
}

void WorldDestroy(World *world) {
    assert(world);

    free(world);
}

int WorldGetWidth(const World *world) {
    assert(world);

    return world->width;
}

int WorldGetHeight(const World *world) {
    assert(world);

    return world->height;
}

int WorldGetBorder(const World *world) {
    assert(world);

    return world->border;
}

Color WorldGetBackgroundColor(const World *world) {
    assert(world);

    return world->background_color;
}

Vector2 WorldResolveBoundaries(const World *world, const Rectangle bounds) {
    assert(world);

    Vector2 resolved = {
        .x = bounds.x,
        .y = bounds.y,
    };

    if (bounds.x < world->border) {
        resolved.x = world->border;
    }

    if (bounds.y < world->border) {
        resolved.y = world->border;
    }

    if (bounds.x > (world->width - bounds.width)) {
        resolved.x = world->width - bounds.width;
    }

    if (bounds.y > (world->height - bounds.height)) {
        resolved.y = world->height - bounds.height;
    }

    return resolved;
}

bool WorldIsOutOfBounds(const World *world, const Rectangle bounds) {
    assert(world);

    if (bounds.x < world->border || bounds.y < world->border || bounds.x > (world->width - bounds.width)
        || bounds.y > (world->height - bounds.height)) {
        return true;
    }

    return false;
}
