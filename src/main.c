#include "asset.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "raylib.h"

int main(void) {
    Game *game = GameCreate(1080, 720, BLACK);
    float game_window_width = GameGetWindowWidth(game);
    float game_window_height = GameGetWindowHeight(game);
    Color game_world_background = GameGetWorldBackground(game);

    InitWindow(game_window_width, game_window_height, "Space War");

    AssetLoadTextures();

    if (!GameAddPlayer(game, PlayerCreate(PLAYER_TYPE_VIPER))) {
        TraceLog(LOG_ERROR, "Failed to add player");

        return 1;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        for (int i = 0; i < GameGetPlayerCount(game); i++) {
            Player *player = GameGetPlayer(game, i);

            PlayerUpdate(player, delta);
            WorldResolveBoundaries(
                game, PlayerGetPosition(player), PlayerGetSize(player));

            for (int j = 0; j < PlayerGetBulletCount(player); j++) {
                Bullet *bullet = PlayerGetBullet(player, j);

                if (WorldIsOutOfBounds(
                        game, BulletGetPosition(bullet),
                        BulletGetSize(bullet))) {
                    PlayerSpliceBullet(player, j);

                    j--;
                    continue;
                }

                BulletUpdate(bullet, delta);
            }
        }

        BeginDrawing();

        ClearBackground(game_world_background);

        for (int i = 0; i < GameGetPlayerCount(game); i++) {
            Player *player = GameGetPlayer(game, i);

            PlayerDraw(player);

            for (int j = 0; j < PlayerGetBulletCount(player); j++) {
                Bullet *bullet = PlayerGetBullet(player, j);

                BulletDraw(bullet);
            }
        }

        EndDrawing();
    }

    GameDestroy(game);
    CloseWindow();

    return 0;
}
