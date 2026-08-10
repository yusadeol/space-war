#pragma once
#include "enemy.h"
#include "player.h"
#include "world.h"

typedef struct Game Game;

constexpr int GAME_WINDOW_WIDTH = 1080;
constexpr int GAME_WINDOW_HEIGHT = 720;
constexpr int GAME_HUD_GAP = 20;

Game *GameCreate(const int window_width, const int window_height);
void GameDestroy(Game *game);

void GameStart(Game *game);
void GameRestart(Game *game);

int GameGetWindowWidth(const Game *game);
int GameGetWindowHeight(const Game *game);
World *GameGetWorld(Game *game);

bool GameAddPlayer(Game *game, Player *player);
Player *GameGetPlayer(Game *game, const int player_index);
int GameGetPlayerCount(const Game *game);
void GameRemovePlayer(Game *game, const int player_index);
void GameRemovePlayers(Game *game, int *player_indexes, const int player_index_count);

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy);
Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index);
int GameGetEnemyCount(const Game *game, const int player_index);
void GameRemoveEnemy(Game *game, const int player_index, const int enemy_index);
void GameRemoveEnemies(Game *game, const int player_index, int *enemy_indexes, const int enemy_index_count);

void GameSpawnRandomEnemiesForPlayer(Game *game, const int player_index);
void GameSpawnRandomEnemiesForPlayers(Game *game);
void GameUpdatePlayers(Game *game, const float delta);
void GameUpdateEnemyies(Game *game, const float delta);
void GameDrawAllBullets(Game *game);
void GameDrawPlayers(Game *game);
void GameDrawEnemies(Game *game);
void GameDrawHud(Game *game);
