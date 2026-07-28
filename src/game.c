#include "game.h"

void WorldResolveBoundaries(const Game *game, Vector2 *position, const Vector2 *size) {
    int border = 10;

    int width_limit = game->window_width - border;
    int height_limit = game->window_height - border;

    if (position->x < border) {
        position->x = border;
    }

    if (position->y < border) {
        position->y = border;
    }

    if (position->x > width_limit - size->x) {
        position->x = width_limit - size->x;
    }

    if (position->y > height_limit - size->y) {
        position->y = height_limit - size->y;
    }
}
