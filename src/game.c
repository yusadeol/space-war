#include "game.h"

void WorldResolveBoundaries(const Game *game, Vector2 *position, Vector2 *size) {
    if (position->x < 0) {
        position->x = 0;
    }

    if (position->y < 0) {
        position->y = 0;
    }

    if (position->x > game->world_size.x - size->x) {
        position->x = game->world_size.x - size->x;
    }

    if (position->y > game->world_size.y - size->y) {
        position->y = game->world_size.y - size->y;
    }
}
