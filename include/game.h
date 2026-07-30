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
bool GameAddPlayer(Game *game, Player *player);
Player *GameGetPlayer(Game *game, int index);

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const TextureSize size);

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 *position, const TextureSize size);

#endif
