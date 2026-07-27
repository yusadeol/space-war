#ifndef GAME_H
#define GAME_H

#include "raylib.h"

typedef struct Game {
    Vector2 world_size;
} Game;

void WorldResolveBoundaries(const Game *game, Vector2 *position, Vector2 *size);

#endif
