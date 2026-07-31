#include "asset.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include <raylib.h>
#include <stdlib.h>

static void UpdatePlayerAndBullets(Game *game, float delta) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        PlayerUpdate(player, delta);
        WorldResolveBoundaries(
            game, PlayerGetPosition(player), PlayerGetSize(player));

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(
                    game, BulletGetPosition(bullet), BulletGetSize(bullet))) {
                PlayerSpliceBullet(player, j);

                j--;
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

static void DrawPlayerAndBullets(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        PlayerDraw(player);

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            BulletDraw(bullet);
        }
    }
}

static void UpdateEnemyAndBullets(Game *game, float delta) {
    for (int i = 0; i < GameGetEnemyCount(game); i++) {
        Enemy *enemy = GameGetEnemy(game, i);

        EnemyUpdate(enemy, delta);
        WorldResolveBoundaries(
            game, EnemyGetPosition(enemy), EnemyGetSize(enemy));

        for (int j = 0; j < EnemyGetBulletCount(enemy); j++) {
            Bullet *bullet = EnemyGetBullet(enemy, j);

            if (WorldIsOutOfBounds(
                    game, BulletGetPosition(bullet), BulletGetSize(bullet))) {
                EnemySpliceBullet(enemy, j);

                j--;
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

static void DrawEnemyAndBullets(Game *game) {
    for (int i = 0; i < GameGetEnemyCount(game); i++) {
        Enemy *enemy = GameGetEnemy(game, i);

        EnemyDraw(enemy);

        for (int j = 0; j < EnemyGetBulletCount(enemy); j++) {
            Bullet *bullet = EnemyGetBullet(enemy, j);

            BulletDraw(bullet);
        }
    }
}

int main(void) {
    Game *game = GameCreate(1080, 720, BLACK);
    if (game == NULL) {
        TraceLog(LOG_ERROR, "Failed to create game");

        return EXIT_FAILURE;
    }

    float game_window_width = GameGetWindowWidth(game);
    float game_window_height = GameGetWindowHeight(game);
    Color game_world_background = GameGetWorldBackground(game);

    InitWindow(game_window_width, game_window_height, "Space War");

    AssetLoadTextures();

    if (!GameAddPlayer(game, PlayerCreate(PLAYER_TYPE_VIPER))) {
        TraceLog(LOG_ERROR, "Failed to add player");

        GameDestroy(game);
        CloseWindow();

        return EXIT_FAILURE;
    }

    if (!GameAddEnemy(game, EnemyCreate(ENEMY_TYPE_SPECTRA))) {
        TraceLog(LOG_ERROR, "Failed to add enemy");

        GameDestroy(game);
        CloseWindow();

        return EXIT_FAILURE;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        UpdatePlayerAndBullets(game, delta);
        UpdateEnemyAndBullets(game, delta);

        BeginDrawing();

        ClearBackground(game_world_background);

        DrawPlayerAndBullets(game);
        DrawEnemyAndBullets(game);

        EndDrawing();
    }

    GameDestroy(game);
    CloseWindow();

    return EXIT_SUCCESS;
}
