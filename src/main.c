#include "asset.h"
#include "collision.h"
#include "game.h"
#include "world.h"
#include <raylib.h>
#include <stdlib.h>

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
    GameSpawnRandomEnemiesForPlayers(game);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        GameUpdatePlayers(game, delta);
        GameUpdateEnemyies(game, delta);
        CollisionUpdate(game);

        BeginDrawing();

        ClearBackground(WorldGetBackgroundColor(GameGetWorld(game)));

        GameDrawHud(game);
        GameDrawAllBullets(game);
        GameDrawPlayers(game);
        GameDrawEnemies(game);

        EndDrawing();
    }

    GameDestroy(game);
    CloseWindow();

    return EXIT_SUCCESS;
}
