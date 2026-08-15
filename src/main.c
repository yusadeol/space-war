#include <raylib.h>
#include <stdlib.h>

#include "collision.h"
#include "game.h"
#include "world.h"

int main(void) {
    Game* game = GameCreate(GAME_WINDOW_WIDTH, GAME_WINDOW_HEIGHT);
    if (game == NULL) {
        return EXIT_FAILURE;
    }

    InitWindow(GameGetWindowWidth(game), GameGetWindowHeight(game), "Space War");
    SetTargetFPS(60);

    GameStart(game);

    while (!WindowShouldClose()) {
        GameHandlePlayerJoins(game);
        GameSpawnRandomEnemiesForPlayers(game);

        float delta = GetFrameTime();

        GameUpdatePlayers(game, delta);
        GameUpdateEnemies(game, delta);
        CollisionUpdate(game);
        GameCull(game);

        BeginDrawing();

        ClearBackground(WorldGetBackgroundColor(GameGetWorld(game)));

        GameDrawPlayers(game);
        GameDrawEnemies(game);
        GameDrawHud(game);

        EndDrawing();
    }

    GameEnd(game);
    GameDestroy(game);

    CloseWindow();

    return EXIT_SUCCESS;
}
