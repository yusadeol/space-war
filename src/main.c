#include "game.h"
#include "player.h"
#include "raylib.h"

int main(void) {
    Game game = {.world_size = {720, 480}};

    InitWindow(game.world_size.x, game.world_size.y, "Space War");

    SetTargetFPS(60);

    Player player = {.size = {40.0f, 80.0f}, .speed = 400.0f};

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        PlayerUpdate(&player, delta);
        WorldResolveBoundaries(&game, &player.position, &player.size);

        BeginDrawing();

        ClearBackground(WHITE);

        PlayerDraw(&player);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
