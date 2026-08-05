#include "world.h"
#include <assert.h>
#include <stdlib.h>

struct World {
    int width;
    int height;
    int border;
    Color background_color;
};

World *WorldCreate(
    const int window_width, const int window_height, const int border,
    Color background_color) {
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

void WorldResolveBoundaries(
    const World *world, Vector2 *position, const Rectangle bounds) {
    assert(world && position);

    if (position->x < world->border) {
        position->x = world->border;
    }

    if (position->y < world->border) {
        position->y = world->border;
    }

    if (position->x > (world->width - bounds.width)) {
        position->x = world->width - bounds.width;
    }

    if (position->y > (world->height - bounds.height)) {
        position->y = world->height - bounds.height;
    }
}

bool WorldIsOutOfBounds(
    const World *world, const Vector2 position, const Rectangle bounds) {
    assert(world);

    if (position.x < world->border || position.y < world->border ||
        position.x > (world->width - bounds.width) ||
        position.y > (world->height - bounds.height)) {
        return true;
    }

    return false;
}
