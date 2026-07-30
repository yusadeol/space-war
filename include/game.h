#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "raylib.h"

typedef struct Game {
    int window_width;
    int window_height;
    Color background;
    Player players[MAX_PLAYERS];
    int player_count;
} Game;

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const Vector2 *size);

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 *position, const Vector2 *size);

#endif
