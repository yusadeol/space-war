#pragma once
#include "enemy.h"
#include "player.h"
#include "world.h"

typedef struct Game Game;

constexpr int GAME_WINDOW_WIDTH = 1080;
constexpr int GAME_WINDOW_HEIGHT = 720;

Game *GameCreate(const int window_width, const int window_height);
void GameDestroy(Game *game);

int GameGetWindowWidth(const Game *game);
int GameGetWindowHeight(const Game *game);
World *GameGetWorld(Game *game);

bool GameAddPlayer(Game *game, Player *player);
Player *GameGetPlayer(Game *game, const int index);
int GameGetPlayerCount(const Game *game);
void GameRemovePlayer(Game *game, const int index);

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy);
void GameSpawnRandomEnemiesForPlayer(Game *game, const int index);
Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index);
int GameGetEnemyCount(const Game *game, const int player_index);
void GameRemoveEnemy(Game *game, const int player_index, const int enemy_index);
