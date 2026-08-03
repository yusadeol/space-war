#include "game.h"
#include "enemy.h"
#include "player.h"
#include <raylib.h>
#include <stdlib.h>

struct Game {
    int window_width;
    int window_height;
    Color world_background;
    Player *players[MAX_PLAYERS];
    int player_count;
    Enemy *enemies[MAX_PLAYERS][MAX_ENEMIES];
    int enemy_count[MAX_PLAYERS];
};

Game *GameCreate() {
    Game *game = malloc(sizeof(*game));

    if (game == NULL) {
        return NULL;
    }

    *game = (Game){
        .window_width = GAME_WINDOW_WIDTH,
        .window_height = GAME_WINDOW_HEIGHT,
        .world_background = GAME_WORLD_BACKGROUND,
    };

    return game;
}

void GameDestroy(Game *game) {
    for (int i = 0; i < game->player_count; i++) {
        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = game->enemies[i][j];

            EnemyDestroy(enemy);
        }

        Player *player = game->players[i];

        PlayerDestroy(player);
    }

    free(game);
}

int GameGetWindowWidth(const Game *game) {
    return game->window_width;
}

int GameGetWindowHeight(const Game *game) {
    return game->window_height;
}

Color GameGetWorldBackground(const Game *game) {
    return game->world_background;
}

bool GameAddPlayer(Game *game, Player *player) {
    if ((game->player_count + 1) > MAX_PLAYERS) {
        return false;
    }

    game->players[game->player_count++] = player;

    return true;
}

Player *GameGetPlayer(Game *game, const int index) {
    if (index < 0 || index >= game->player_count) {
        return NULL;
    }

    return game->players[index];
}

int GameGetPlayerCount(const Game *game) {
    return game->player_count;
}

void GameSplicePlayer(Game *game, const int index) {
    Player *player = game->players[index];
    PlayerDestroy(player);

    for (int i = index; i < game->player_count - 1; i++) {
        game->players[i] = game->players[i + 1];
    }

    game->player_count--;
}

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy) {
    if ((game->enemy_count[player_index] + 1) > MAX_ENEMIES) {
        return false;
    }

    game->enemies[player_index][game->enemy_count[player_index]++] = enemy;

    return true;
}

void GameCreateRandomEnemiesForPlayer(Game *game, const int index) {
    int enemy_amount = GetRandomValue(1, MAX_ENEMIES);
    for (int i = 0; i < enemy_amount; i++) {
        if (!GameAddEnemy(game, index, EnemyCreate(ENEMY_TYPE_SPECTRA))) {
            TraceLog(
                LOG_WARNING, "Failed to add enemy %d for player %d", i, index);
        }
    }
}

Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index) {
    if (enemy_index < 0 || enemy_index >= game->enemy_count[player_index]) {
        return NULL;
    }

    return game->enemies[player_index][enemy_index];
}

int GameGetEnemyCount(const Game *game, const int player_index) {
    return game->enemy_count[player_index];
}

void GameSpliceEnemy(
    Game *game, const int player_index, const int enemy_index) {
    Enemy *enemy = game->enemies[player_index][enemy_index];
    EnemyDestroy(enemy);

    for (int i = enemy_index; i < game->enemy_count[player_index] - 1; i++) {
        game->enemies[player_index][i] = game->enemies[player_index][i + 1];
    }

    game->enemy_count[player_index]--;
}

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const Rectangle bounds) {
    int width_limit = game->window_width - GAME_WORLD_BORDER;
    int height_limit = game->window_height - GAME_WORLD_BORDER;

    if (position->x < GAME_WORLD_BORDER) {
        position->x = GAME_WORLD_BORDER;
    }

    if (position->y < GAME_WORLD_BORDER) {
        position->y = GAME_WORLD_BORDER;
    }

    if (position->x > (width_limit - bounds.width)) {
        position->x = width_limit - bounds.width;
    }

    if (position->y > (height_limit - bounds.height)) {
        position->y = height_limit - bounds.height;
    }
}

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 position, const Rectangle bounds) {
    int width_limit = game->window_width - GAME_WORLD_BORDER;
    int height_limit = game->window_height - GAME_WORLD_BORDER;

    if (position.x < GAME_WORLD_BORDER || position.y < GAME_WORLD_BORDER ||
        position.x > (width_limit - bounds.width) ||
        position.y > (height_limit - bounds.height)) {
        return true;
    }

    return false;
}
