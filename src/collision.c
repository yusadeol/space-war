#include "collision.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"
#include <raylib.h>

static void CollisionPlayerBulletsVsEnemies(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        Player *player = GameGetPlayer(game, i);

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            for (int k = 0; k < GameGetEnemyCount(game, i); k++) {
                Enemy *enemy = GameGetEnemy(game, i, k);

                if (CheckCollisionRecs(
                        BulletGetBounds(bullet), EnemyGetBounds(enemy))) {
                    PlayerSpliceBullet(player, j);
                    GameSpliceEnemy(game, i, k);

                    j--;
                    break;
                }
            }
        }
    }
}

static void CollisionEnemyBulletsVsPlayers(Game *game) {
    for (int i = 0; i < GameGetPlayerCount(game); i++) {
        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                for (int l = 0; l < GameGetPlayerCount(game); l++) {
                    Player *player = GameGetPlayer(game, l);

                    if (CheckCollisionRecs(
                            BulletGetBounds(bullet), PlayerGetBounds(player))) {
                        EnemySpliceBullet(enemy, k);
                        GameSplicePlayer(game, l);

                        k--;
                        break;
                    }
                }
            }
        }
    }
}

void CollisionUpdate(Game *game) {
    CollisionPlayerBulletsVsEnemies(game);
    CollisionEnemyBulletsVsPlayers(game);
}
