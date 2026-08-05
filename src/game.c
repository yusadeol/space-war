#include "game.h"
#include "bullet.h"
#include "controller.h"
#include "enemy.h"
#include "player.h"
#include "world.h"
#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Game {
    int window_width;
    int window_height;
    World *world;
    Player *players[MAX_PLAYERS];
    int player_count;
    Enemy *enemies[MAX_PLAYERS][MAX_ENEMIES];
    int enemy_count[MAX_PLAYERS];
};

Game *GameCreate(const int window_width, const int window_height) {
    Game *game = malloc(sizeof(*game));

    if (game == NULL) {
        return NULL;
    }

    *game = (Game){
        .window_width = window_width,
        .window_height = window_height,
    };

    game->world = WorldCreate(
        game->window_width, game->window_height, WORLD_BORDER,
        WORLD_BACKGROUND_COLOR);

    return game;
}

static void Clear(Game *game) {
    for (int i = 0; i < game->player_count; i++) {
        GameRemovePlayer(game, i);

        i--;
    }
}

void GameDestroy(Game *game) {
    assert(game);

    Clear(game);
    WorldDestroy(game->world);
    free(game);
}

void GameStart(Game *game) {
    assert(game);

    if (!GameAddPlayer(game, PlayerCreate(PLAYER_TYPE_VIPER))) {
        TraceLog(LOG_ERROR, "Failed to add player");
    }
}

void GameRestart(Game *game) {
    assert(game);

    Clear(game);
    GameStart(game);
}

int GameGetWindowWidth(const Game *game) {
    assert(game);

    return game->window_width;
}

int GameGetWindowHeight(const Game *game) {
    assert(game);

    return game->window_height;
}

World *GameGetWorld(Game *game) {
    assert(game);

    return game->world;
}

bool GameAddPlayer(Game *game, Player *player) {
    assert(game && player);

    if ((game->player_count + 1) > MAX_PLAYERS) {
        return false;
    }

    game->players[game->player_count++] = player;

    return true;
}

Player *GameGetPlayer(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= game->player_count) {
        return NULL;
    }

    return game->players[player_index];
}

int GameGetPlayerCount(const Game *game) {
    assert(game);

    return game->player_count;
}

void GameRemovePlayer(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= game->player_count) {
        return;
    }

    Player *player = game->players[player_index];

    for (int i = 0; game->enemy_count[player_index]; i++) {
        GameRemoveEnemy(game, player_index, i);

        i--;
    }

    PlayerDestroy(player);

    for (int i = player_index; i < game->player_count - 1; i++) {
        game->players[i] = game->players[i + 1];
        game->enemy_count[i] = game->enemy_count[i + 1];

        for (int j = 0; j < game->enemy_count[i + 1]; j++) {
            game->enemies[i][j] = game->enemies[i + 1][j];
        }
    }

    game->player_count--;
}

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy) {
    assert(game && enemy);

    if (player_index < 0 || player_index >= game->player_count) {
        return false;
    }

    if ((game->enemy_count[player_index] + 1) > MAX_ENEMIES) {
        return false;
    }

    game->enemies[player_index][game->enemy_count[player_index]++] = enemy;

    return true;
}

Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index) {
    assert(game);

    if (player_index < 0 || player_index >= game->player_count) {
        return NULL;
    }

    if (enemy_index < 0 || enemy_index >= game->enemy_count[player_index]) {
        return NULL;
    }

    return game->enemies[player_index][enemy_index];
}

int GameGetEnemyCount(const Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= game->player_count) {
        return 0;
    }

    return game->enemy_count[player_index];
}

void GameRemoveEnemy(
    Game *game, const int player_index, const int enemy_index) {
    assert(game);

    if (player_index < 0 || player_index >= game->player_count) {
        return;
    }

    if (enemy_index < 0 || enemy_index >= game->enemy_count[player_index]) {
        return;
    }

    Enemy *enemy = game->enemies[player_index][enemy_index];
    EnemyDestroy(enemy);

    for (int i = enemy_index; i < game->enemy_count[player_index] - 1; i++) {
        game->enemies[player_index][i] = game->enemies[player_index][i + 1];
    }

    game->enemy_count[player_index]--;
}

void GameSpawnRandomEnemiesForPlayer(Game *game, const int player_index) {
    assert(game);

    int enemy_amount = GetRandomValue(1, MAX_ENEMIES);
    for (int i = 0; i < enemy_amount; i++) {
        if (!GameAddEnemy(
                game, player_index,
                EnemyCreate(
                    ENEMY_TYPE_SPECTRA, game->window_width,
                    WorldGetHeight(game->world),
                    WorldGetBorder(game->world)))) {
            TraceLog(
                LOG_WARNING, "Failed to add enemy %d for player %d", i,
                player_index);
        }
    }
}

void GameSpawnRandomEnemiesForPlayers(Game *game) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        GameSpawnRandomEnemiesForPlayer(game, i);
    }
}

void GameUpdatePlayers(Game *game, const float delta) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        Player *player = GameGetPlayer(game, i);

        ControllerInput input = IsGamepadAvailable(i)
                                    ? ControllerGetGamepadInput(i)
                                    : ControllerGetKeyboardInput();

        PlayerUpdate(player, input, delta);
        WorldResolveBoundaries(
            game->world, PlayerGetPosition(player), PlayerGetBounds(player));

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(
                    game->world, *BulletGetPosition(bullet),
                    BulletGetBounds(bullet))) {
                PlayerRemoveBullet(player, j);

                j--;
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

void GameUpdateEnemyies(Game *game, const float delta) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            EnemyUpdate(
                enemy, game->window_height, WorldGetWidth(game->world),
                *PlayerGetPosition(player), PlayerGetCenterPosition(player),
                delta);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                if (WorldIsOutOfBounds(
                        game->world, *BulletGetPosition(bullet),
                        BulletGetBounds(bullet))) {
                    EnemyRemoveBullet(enemy, k);

                    k--;
                    continue;
                }

                BulletUpdate(bullet, delta);
            }
        }
    }
}

void GameDrawAllBullets(Game *game) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            BulletDraw(bullet);
        }

        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                BulletDraw(bullet);
            }
        }
    }
}

void GameDrawPlayers(Game *game) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        Player *player = GameGetPlayer(game, i);

        PlayerDraw(player);
    }
}

void GameDrawEnemies(Game *game) {
    assert(game);

    for (int i = 0; i < game->player_count; i++) {
        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            EnemyDraw(enemy);
        }
    }
}

void GameDrawHud(Game *game) {
    assert(game);

    int world_border = WorldGetBorder(game->world);
    int cursor_x = world_border;

    for (int i = 0; i < game->player_count; i++) {
        Player *player = GameGetPlayer(game, i);

        const char *text = TextFormat(
            "Player %d kill count: %d", i + 1, PlayerGetKillCount(player));

        DrawText(text, cursor_x + (GAME_HUD_GAP * i), world_border, 14, WHITE);
        cursor_x += MeasureText(text, 14);
    }
}
