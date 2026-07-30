#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "raylib.h"

typedef struct Game Game;

Game *GameCreate(int window_width, int window_height, Color background);
void GameDestroy(Game *game);

int GameGetWindowWidth(const Game *game);
int GameGetWindowHeight(const Game *game);
Color GameGetWorldBackground(const Game *game);
int GameGetPlayerCount(const Game *game);
Player *GameGetPlayer(Game *game, int index);
bool GameAddPlayer(Game *game, const Player *player);

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const Vector2 *size);

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 *position, const Vector2 *size);

#endif
