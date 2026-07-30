#include "asset.h"
#include "bullet.h"
#include "game.h"
#include "player.h"
#include "raylib.h"

int main(void) {
    Game game = {.window_width = 1080,
                 .window_height = 720,
                 .background = BLACK};

    InitWindow(game.window_width, game.window_height, "Space War");

    AssetLoadTextures();

    game.players[game.player_count++] =
        (Player){.texture = *AssetGetTexture(TEXTURE_SPACESHIP_VIPER)};

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();

        for (int i = 0; i < game.player_count; i++) {
            Player *player = &game.players[i];

            PlayerUpdate(player, delta);
            WorldResolveBoundaries(
                &game, &player->position,
                &(Vector2){player->texture.width, player->texture.height});

            for (int j = 0; j < player->bullet_count; j++) {
                Bullet *bullet = &player->bullets[j];

                if (WorldIsOutOfBounds(
                        &game, &bullet->position,
                        &(Vector2){bullet->texture.width,
                                   bullet->texture.height})) {
                    PlayerSpliceBullet(player, j);

                    j--;
                    continue;
                }

                BulletUpdate(bullet, delta);
            }
        }

        BeginDrawing();

        ClearBackground(game.background);

        for (int i = 0; i < game.player_count; i++) {
            Player *player = &game.players[i];

            PlayerDraw(player);

            for (int j = 0; j < player->bullet_count; j++) {
                Bullet *bullet = &player->bullets[j];

                BulletDraw(bullet);
            }
        }

        EndDrawing();
    }

    CloseWindow();

    return 0;
}
