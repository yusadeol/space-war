#pragma once
#include "enemy.h"
#include "player.h"
#include <raylib.h>

typedef struct Game Game;

constexpr int GAME_WINDOW_WIDTH = 1080;
constexpr int GAME_WINDOW_HEIGHT = 720;
constexpr int GAME_WORLD_BORDER = 5;
constexpr Color GAME_WORLD_BACKGROUND = {0, 0, 0, 255};

Game *GameCreate();
void GameDestroy(Game *game);

int GameGetWindowWidth(const Game *game);
int GameGetWindowHeight(const Game *game);
Color GameGetWorldBackground(const Game *game);

bool GameAddPlayer(Game *game, Player *player);
Player *GameGetPlayer(Game *game, const int index);
int GameGetPlayerCount(const Game *game);
void GameSplicePlayer(Game *game, const int index);

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy);
void GameCreateRandomEnemiesForPlayer(Game *game, const int index);
Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index);
int GameGetEnemyCount(const Game *game, const int player_index);
void GameSpliceEnemy(Game *game, const int player_index, const int enemy_index);

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const Rectangle bounds);

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 position, const Rectangle bounds);
