#include "game.h"
#include "player.h"
#include "raylib.h"

int main(void) {
    Game game = {720, 480, BLACK};

    InitWindow(game.window_width, game.window_height, "Space War");

    SetTargetFPS(60);

    Player player = {
        .texture = LoadTexture("assets/textures/spaceships/viper.png"), .scale = {1.0f, 1.0f}, .speed = 400.0f};

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        PlayerUpdate(&player, delta);
        WorldResolveBoundaries(&game, &player.position, (Vector2){player.texture.width, player.texture.height});

        BeginDrawing();

        ClearBackground(game.background);

        PlayerDraw(&player);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
