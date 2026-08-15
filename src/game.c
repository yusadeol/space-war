#include "game.h"

#include "array.h"
#include "asset.h"
#include "bullet.h"
#include "controller.h"
#include "enemy.h"
#include "gamepad.h"
#include "keyboard.h"
#include "player.h"
#include "world.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static const PlayerType player_types[MAX_PLAYERS] = {PLAYER_TYPE_VIPER, PLAYER_TYPE_RAPTOR};

struct Game {
    int window_width;
    int window_height;
    World *world;
    Player *players[MAX_PLAYERS];
    Controller *player_controllers[MAX_PLAYERS];
    Enemy *enemies[MAX_PLAYERS][MAX_ENEMIES];
    int enemy_count[MAX_PLAYERS];
};

Game *GameCreate(const int window_width, const int window_height) {
    World *world = WorldCreate(window_width, window_height, WORLD_BORDER, WORLD_BACKGROUND_COLOR);
    if (world == NULL) {
        TraceLog(LOG_ERROR, "Failed to create world");

        return NULL;
    }

    Game *game = malloc(sizeof(*game));

    if (game == NULL) {
        TraceLog(LOG_ERROR, "Failed to create game");
        WorldDestroy(world);

        return NULL;
    }

    *game = (Game){
        .window_width = window_width,
        .window_height = window_height,
        .world = world,
    };

    return game;
}

void GameDestroy(Game *game) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        (void)GameRemovePlayerController(game, i);
        (void)GameRemovePlayer(game, i);
    }

    WorldDestroy(game->world);
    free(game);
}

static Controller *CreatePlayerController(int player_index) {
    Controller *controller = (Controller *)GamepadCreate(player_index);

    if (controller == NULL && player_index == 0) {
        controller = (Controller *)KeyboardCreate();
    }

    return controller;
}

void GameStart(Game *game) {
    assert(game);

    AssetLoadTextures();
    AssetLoadMetadatas();

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->player_controllers[i] != NULL) {
            continue;
        }

        Controller *controller = CreatePlayerController(i);
        if (controller == NULL) {
            TraceLog(LOG_ERROR, "Failed to create controller for player %d", i);

            continue;
        }

        if (!GameAddPlayerController(game, i, controller)) {
            TraceLog(LOG_ERROR, "Failed to add controller for player %d", i);

            controller->Destroy(controller);
        }
    }
}

void GameEnd(Game *game) {
    assert(game);

    AssetUnloadTextures();
    AssetUnloadMetadatas();
}

void GameHandlePlayerJoins(Game *game) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] != NULL) {
            continue;
        }

        Controller *controller = game->player_controllers[i];
        if (controller == NULL) {
            continue;
        }

        ControllerInput input = controller->GetInput(controller);
        if (input.start) {
            Player *player = PlayerCreate(player_types[i]);
            if (player == NULL) {
                TraceLog(LOG_ERROR, "Failed to create player %d", i);

                continue;
            }

            if (!GameAddPlayer(game, i, player)) {
                TraceLog(LOG_ERROR, "Failed to add player %d", i);

                PlayerDestroy(player);
            }
        }
    }
}

int GameGetWindowWidth(const Game *game) {
    assert(game);

    return game->window_width;
}

int GameGetWindowHeight(const Game *game) {
    assert(game);

    return game->window_height;
}

const World *GameGetWorld(const Game *game) {
    assert(game);

    return game->world;
}

bool GameAddPlayer(Game *game, const int player_index, Player *player) {
    assert(game && player);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    if (game->players[player_index] != NULL) {
        TraceLog(LOG_WARNING, "Player slot %d is already occupied", player_index);

        return false;
    }

    game->players[player_index] = player;

    return true;
}

Player *GameGetPlayer(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return NULL;
    }

    return game->players[player_index];
}

bool GameRemovePlayer(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    for (int i = 0; game->enemy_count[player_index]; i++) {
        (void)GameRemoveEnemy(game, player_index, i);

        i--;
    }

    Player *player = game->players[player_index];
    if (player != NULL) {
        PlayerDestroy(player);
    }
    game->players[player_index] = NULL;

    return true;
}

bool GameRemovePlayers(Game *game, int *player_indexes, const int player_index_count) {
    assert(game);

    qsort(player_indexes, player_index_count, sizeof(*player_indexes), ArrayCompareIntegerAscending);

    bool all_ok = true;

    for (int i = player_index_count - 1; i >= 0; i--) {
        if (!GameRemovePlayer(game, player_indexes[i])) {
            TraceLog(LOG_ERROR, "Failed to remove player at index %d", player_indexes[i]);
            all_ok = false;
        }
    }

    return all_ok;
}

bool GameAddPlayerController(Game *game, const int player_index, Controller *controller) {
    assert(game && controller);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    if (game->player_controllers[player_index] != NULL) {
        TraceLog(LOG_WARNING, "Player controller slot %d is already occupied", player_index);

        return false;
    }

    game->player_controllers[player_index] = controller;

    return true;
}

Controller *GameGetPlayerController(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return NULL;
    }

    return game->player_controllers[player_index];
}

bool GameRemovePlayerController(Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    Controller *controller = game->player_controllers[player_index];
    if (controller != NULL) {
        controller->Destroy(controller);
    }
    game->player_controllers[player_index] = NULL;

    return true;
}

bool GameAddEnemy(Game *game, const int player_index, Enemy *enemy) {
    assert(game && enemy);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    if (game->players[player_index] == NULL) {
        TraceLog(LOG_WARNING, "Cannot add enemy for player %d: player does not exist", player_index);

        return false;
    }

    if ((game->enemy_count[player_index] + 1) > MAX_ENEMIES) {
        TraceLog(LOG_WARNING, "Enemy capacity reached for player %d (%d)", player_index, MAX_ENEMIES);

        return false;
    }

    game->enemies[player_index][game->enemy_count[player_index]++] = enemy;

    return true;
}

Enemy *GameGetEnemy(Game *game, const int player_index, const int enemy_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return NULL;
    }

    if (enemy_index < 0 || enemy_index >= game->enemy_count[player_index]) {
        return NULL;
    }

    return game->enemies[player_index][enemy_index];
}

int GameGetEnemyCount(const Game *game, const int player_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return 0;
    }

    return game->enemy_count[player_index];
}

bool GameRemoveEnemy(Game *game, const int player_index, const int enemy_index) {
    assert(game);

    if (player_index < 0 || player_index >= MAX_PLAYERS) {
        return false;
    }

    if (enemy_index < 0 || enemy_index >= game->enemy_count[player_index]) {
        return false;
    }

    Enemy *enemy = game->enemies[player_index][enemy_index];
    EnemyDestroy(enemy);

    for (int i = enemy_index; i < game->enemy_count[player_index] - 1; i++) {
        game->enemies[player_index][i] = game->enemies[player_index][i + 1];
    }

    game->enemy_count[player_index]--;

    return true;
}

bool GameRemoveEnemies(Game *game, const int player_index, int *enemy_indexes, const int enemy_index_count) {
    assert(game);

    qsort(enemy_indexes, enemy_index_count, sizeof(*enemy_indexes), ArrayCompareIntegerAscending);

    bool all_ok = true;

    for (int i = enemy_index_count - 1; i >= 0; i--) {
        if (!GameRemoveEnemy(game, player_index, enemy_indexes[i])) {
            TraceLog(LOG_ERROR, "Failed to remove enemy at index %d for player %d", enemy_indexes[i], player_index);
            all_ok = false;
        }
    }

    return all_ok;
}

void GameSpawnRandomEnemiesForPlayer(Game *game, const int player_index) {
    assert(game);

    int enemy_amount = GetRandomValue(1, MAX_ENEMIES);
    for (int i = 0; i < enemy_amount; i++) {
        Enemy *enemy = EnemyCreate(ENEMY_TYPE_SPECTRA, game->window_width, WorldGetHeight(game->world),
            WorldGetBorder(game->world));
        if (enemy == NULL) {
            TraceLog(LOG_WARNING, "Failed to create enemy %d for player %d", i, player_index);

            continue;
        }

        if (!GameAddEnemy(game, player_index, enemy)) {
            TraceLog(LOG_WARNING, "Failed to add enemy %d for player %d", i, player_index);

            EnemyDestroy(enemy);
        }
    }
}

void GameSpawnRandomEnemiesForPlayers(Game *game) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] != NULL && game->enemy_count[i] == 0) {
            GameSpawnRandomEnemiesForPlayer(game, i);
        }
    }
}

void GameUpdatePlayers(Game *game, const float delta) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = game->players[i];
        if (player == NULL) {
            continue;
        }

        Controller *controller = game->player_controllers[i];
        if (controller == NULL) {
            continue;
        }

        PlayerUpdate(player, controller->GetInput(controller), delta);

        if (PlayerGetStatus(player) == PLAYER_STATUS_NORMAL) {
            PlayerSetPosition(player, WorldResolveBoundaries(game->world, PlayerGetBounds(player)));
        }

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(game->world, BulletGetBounds(bullet))) {
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

void GameUpdateEnemies(Game *game, const float delta) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = game->players[i];
        if (player == NULL) {
            continue;
        }

        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = game->enemies[i][j];

            EnemyUpdate(enemy, game->window_height, WorldGetWidth(game->world), PlayerGetPosition(player),
                PlayerGetCenterPosition(player), delta);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                if (WorldIsOutOfBounds(game->world, BulletGetBounds(bullet))) {
                    continue;
                }

                BulletUpdate(bullet, delta);
            }
        }
    }
}

void GameCull(Game *game) {
    int exploded_players[MAX_PLAYERS] = {};
    int exploded_player_count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = game->players[i];
        if (player == NULL) {
            continue;
        }

        if (PlayerGetStatus(player) == PLAYER_STATUS_EXPLODED) {
            exploded_players[exploded_player_count++] = i;

            continue;
        }

        int destroyed_bullets[MAX_SIMULTANEOUS_BULLETS] = {};
        int destroyed_bullet_count = 0;

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(game->world, BulletGetBounds(bullet))
                || BulletGetStatus(bullet) == BULLET_STATUS_DESTROYED) {
                destroyed_bullets[destroyed_bullet_count++] = j;

                continue;
            }
        }

        PlayerRemoveBullets(player, destroyed_bullets, destroyed_bullet_count);

        int exploded_enemies[MAX_ENEMIES] = {};
        int exploded_enemy_count = 0;

        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = game->enemies[i][j];

            if (EnemyGetStatus(enemy) == ENEMY_STATUS_EXPLODED) {
                exploded_enemies[exploded_enemy_count++] = j;

                continue;
            }

            int destroyed_bullets[MAX_SIMULTANEOUS_BULLETS] = {};
            int destroyed_bullet_count = 0;

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                if (WorldIsOutOfBounds(game->world, BulletGetBounds(bullet))
                    || BulletGetStatus(bullet) == BULLET_STATUS_DESTROYED) {
                    destroyed_bullets[destroyed_bullet_count++] = k;

                    continue;
                }
            }

            EnemyRemoveBullets(enemy, destroyed_bullets, destroyed_bullet_count);
        }

        GameRemoveEnemies(game, i, exploded_enemies, exploded_enemy_count);
    }

    GameRemovePlayers(game, exploded_players, exploded_player_count);
}

void GameDrawPlayers(Game *game) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = game->players[i];
        if (player == NULL) {
            continue;
        }

        PlayerDraw(player);

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            BulletDraw(bullet);
        }
    }
}

void GameDrawEnemies(Game *game) {
    assert(game);

    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (game->players[i] == NULL) {
            continue;
        }

        for (int j = 0; j < game->enemy_count[i]; j++) {
            Enemy *enemy = game->enemies[i][j];

            EnemyDraw(enemy);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                BulletDraw(bullet);
            }
        }
    }
}

void GameDrawHud(Game *game) {
    assert(game);

    int world_border = WorldGetBorder(game->world);
    int cursor_x = world_border;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        const char *text;

        Player *player = game->players[i];
        if (player == NULL) {
            text = TextFormat("Player %d: Press START to join", i + 1);
        } else {
            text = TextFormat("Player %d kill count: %d", i + 1, PlayerGetKillCount(player));
        }

        DrawText(text, cursor_x + (GAME_HUD_GAP * i), world_border, 14, WHITE);
        cursor_x += MeasureText(text, 14);
    }
}
