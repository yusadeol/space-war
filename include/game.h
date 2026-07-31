#pragma once
#include "asset.h"
#include "enemy.h"
#include "player.h"
#include <raylib.h>

typedef struct Game Game;

Game *GameCreate(
    const int window_width, const int window_height,
    const Color world_background);
void GameDestroy(Game *game);

int GameGetWindowWidth(const Game *game);
int GameGetWindowHeight(const Game *game);
Color GameGetWorldBackground(const Game *game);

bool GameAddPlayer(Game *game, Player *player);
Player *GameGetPlayer(Game *game, const int index);
int GameGetPlayerCount(const Game *game);

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy);
Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index);
int GameGetEnemyCount(const Game *game, const int player_index);

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const TextureSize size);

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 position, const TextureSize size);
