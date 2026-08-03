#include "world.h"
#include <stdlib.h>

struct World {
    int width;
    int height;
    Color background_color;
};

World *WorldCreate(
    const int window_width, const int window_height, Color background_color) {
    World *world = malloc(sizeof(*world));

    if (world == NULL) {
        return NULL;
    }

    *world = (World){
        .width = window_width - WORLD_BORDER,
        .height = window_height - WORLD_BORDER,
        .background_color = background_color,
    };

    return world;
}

void WorldDestroy(World *world) {
    free(world);
}

Color WorldGetBackgroundColor(const World *world) {
    return world->background_color;
}

void WorldResolveBoundaries(
    const World *world, Vector2 *position, const Rectangle bounds) {
    if (position->x < WORLD_BORDER) {
        position->x = WORLD_BORDER;
    }

    if (position->y < WORLD_BORDER) {
        position->y = WORLD_BORDER;
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
    if (position.x < WORLD_BORDER || position.y < WORLD_BORDER ||
        position.x > (world->width - bounds.width) ||
        position.y > (world->height - bounds.height)) {
        return true;
    }

    return false;
}
