#include "game.h"
#include "enemy.h"
#include "player.h"
#include <stdlib.h>

static const int border = 10;

struct Game {
    int window_width;
    int window_height;
    Color world_background;
    Player *players[MAX_PLAYERS];
    int player_count;
    Enemy *enemies[MAX_ENEMIES];
    int enemy_count;
};

Game *GameCreate(int window_width, int window_height, Color world_background) {
    Game *game = malloc(sizeof(*game));

    if (game == NULL) {
        return NULL;
    }

    *game = (Game){
        .window_width = window_width,
        .window_height = window_height,
        .world_background = world_background,
    };

    return game;
}

void GameDestroy(Game *game) {
    for (int i = 0; i < game->player_count; i++) {
        Player *player = game->players[i];

        PlayerDestroy(player);
    }

    for (int i = 0; i < game->enemy_count; i++) {
        Enemy *enemy = game->enemies[i];

        EnemyDestroy(enemy);
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

Player *GameGetPlayer(Game *game, int index) {
    if (index < 0 || index >= game->player_count) {
        return NULL;
    }

    return game->players[index];
}

int GameGetPlayerCount(const Game *game) {
    return game->player_count;
}

bool GameAddEnemy(Game *game, Enemy *enemy) {
    if ((game->enemy_count + 1) > MAX_ENEMIES) {
        return false;
    }

    game->enemies[game->enemy_count++] = enemy;

    return true;
}

Enemy *GameGetEnemy(Game *game, int index) {
    if (index < 0 || index >= game->enemy_count) {
        return NULL;
    }

    return game->enemies[index];
}

int GameGetEnemyCount(const Game *game) {
    return game->enemy_count;
}

void WorldResolveBoundaries(
    const Game *game, Vector2 *position, const TextureSize size) {
    int width_limit = game->window_width - border;
    int height_limit = game->window_height - border;

    if (position->x < border) {
        position->x = border;
    }

    if (position->y < border) {
        position->y = border;
    }

    if (position->x > (width_limit - size.width)) {
        position->x = width_limit - size.width;
    }

    if (position->y > (height_limit - size.height)) {
        position->y = height_limit - size.height;
    }
}

bool WorldIsOutOfBounds(
    const Game *game, const Vector2 *position, const TextureSize size) {
    int width_limit = game->window_width - border;
    int height_limit = game->window_height - border;

    if (position->x < border || position->y < border ||
        position->x > (width_limit - size.width) ||
        position->y > (height_limit - size.height)) {
        return true;
    }

    return false;
}
