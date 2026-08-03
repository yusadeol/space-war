#include "asset.h"
#include "bullet.h"
#include "collision.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include <raylib.h>
#include <stdlib.h>

static void CreateRandomEnemiesForPlayers(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        GameCreateRandomEnemiesForPlayer(game, i);
    }
}

static void UpdatePlayersAndBullets(Game *game, const float delta) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        PlayerUpdate(player, delta);
        WorldResolveBoundaries(
            game, PlayerGetPosition(player), PlayerGetBounds(player));

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(
                    game, *BulletGetPosition(bullet),
                    BulletGetBounds(bullet))) {
                PlayerSpliceBullet(player, j);

                j--;
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

static void UpdateEnemiesAndBullets(Game *game, const float delta) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            EnemyUpdate(
                enemy, *PlayerGetPosition(player),
                PlayerGetCenterPosition(player), delta);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                if (WorldIsOutOfBounds(
                        game, *BulletGetPosition(bullet),
                        BulletGetBounds(bullet))) {
                    EnemySpliceBullet(enemy, k);

                    k--;
                    continue;
                }

                BulletUpdate(bullet, delta);
            }
        }
    }
}

static void DrawPlayers(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        PlayerDraw(player);
    }
}

static void DrawEnemies(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            EnemyDraw(enemy);
        }
    }
}

static void DrawBullets(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            BulletDraw(bullet);
        }

        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                BulletDraw(bullet);
            }
        }
    }
}

int main(void) {
    Game *game = GameCreate();
    if (game == NULL) {
        TraceLog(LOG_ERROR, "Failed to create game");

        return EXIT_FAILURE;
    }

    InitWindow(
        GameGetWindowWidth(game), GameGetWindowHeight(game), "Space War");

    AssetLoadTextures();

    if (!GameAddPlayer(game, PlayerCreate(PLAYER_TYPE_VIPER))) {
        TraceLog(LOG_ERROR, "Failed to add player");

        GameDestroy(game);
        CloseWindow();

        return EXIT_FAILURE;
    }

    CreateRandomEnemiesForPlayers(game);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        UpdatePlayersAndBullets(game, delta);
        UpdateEnemiesAndBullets(game, delta);
        CollisionUpdate(game);

        BeginDrawing();

        ClearBackground(GameGetWorldBackground(game));

        DrawPlayers(game);
        DrawEnemies(game);
        DrawBullets(game);

        EndDrawing();
    }

    GameDestroy(game);
    CloseWindow();

    return EXIT_SUCCESS;
}
