#include "asset.h"
#include "bullet.h"
#include "collision.h"
#include "controller.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include "world.h"
#include <raylib.h>
#include <stdlib.h>

static void SpawnRandomEnemiesForPlayers(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        GameSpawnRandomEnemiesForPlayer(game, i);
    }
}

static void UpdatePlayers(Game *game, const float delta) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        ControllerInput input = IsGamepadAvailable(i)
                                    ? ControllerGetGamepadInput(i)
                                    : ControllerGetKeyboardInput();

        PlayerUpdate(player, input, delta);
        WorldResolveBoundaries(
            GameGetWorld(game), PlayerGetPosition(player),
            PlayerGetBounds(player));

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            if (WorldIsOutOfBounds(
                    GameGetWorld(game), *BulletGetPosition(bullet),
                    BulletGetBounds(bullet))) {
                PlayerRemoveBullet(player, j);

                j--;
                continue;
            }

            BulletUpdate(bullet, delta);
        }
    }
}

static void UpdateEnemyies(Game *game, const float delta) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            EnemyUpdate(
                enemy, GameGetWindowHeight(game),
                WorldGetWidth(GameGetWorld(game)), *PlayerGetPosition(player),
                PlayerGetCenterPosition(player), delta);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                if (WorldIsOutOfBounds(
                        GameGetWorld(game), *BulletGetPosition(bullet),
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

static void DrawAllBullets(Game *game) {
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

static void DrawHud(Game *game) {
    int world_border = WorldGetBorder(GameGetWorld(game));
    int cursor_position_x = world_border;
    int cursor_position_y = world_border;
    int gap = 20;

    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        const char *text = TextFormat(
            "Player %d kill count: %d", i + 1, PlayerGetKillCount(player));

        DrawText(
            text, cursor_position_x + (gap * i), cursor_position_y, 14, WHITE);
        cursor_position_x += MeasureText(text, 14);
    }
}

int main(void) {
    Game *game = GameCreate(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    if (game == NULL) {
        TraceLog(LOG_ERROR, "Failed to create game");

        return EXIT_FAILURE;
    }

    InitWindow(
        GameGetWindowWidth(game), GameGetWindowHeight(game), "Space War");

    AssetLoadTextures();
    GameStart(game);
    SpawnRandomEnemiesForPlayers(game);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        UpdatePlayers(game, delta);
        UpdateEnemyies(game, delta);
        CollisionUpdate(game);

        BeginDrawing();

        ClearBackground(WorldGetBackgroundColor(GameGetWorld(game)));

        DrawHud(game);
        DrawAllBullets(game);
        DrawPlayers(game);
        DrawEnemies(game);

        EndDrawing();
    }

    GameDestroy(game);
    CloseWindow();

    return EXIT_SUCCESS;
}
