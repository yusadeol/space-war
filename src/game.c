#include "game.h"

static const int border = 10;

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const Vector2 *size) {
    int width_limit = game->window_width - border;
    int height_limit = game->window_height - border;

    if (position->x < border) {
        position->x = border;
    }

    if (position->y < border) {
        position->y = border;
    }

    if (position->x > (width_limit - size->x)) {
        position->x = width_limit - size->x;
    }

    if (position->y > (height_limit - size->y)) {
        position->y = height_limit - size->y;
    }
}

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 *position, const Vector2 *size) {
    int width_limit = game->window_width - border;
    int height_limit = game->window_height - border;

    if (position->x < border || position->y < border ||
        position->x > (width_limit - size->x) ||
        position->y > (height_limit - size->y)) {
        return true;
    }

    return false;
}
