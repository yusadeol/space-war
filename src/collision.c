#include "collision.h"

#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static void ResolvePlayerBulletCollisions(Game *game) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        Player *player = GameGetPlayer(game, i);
        if (player == NULL) {
            continue;
        }

        for (int j = 0; j < PlayerGetBulletCount(player); j++) {
            Bullet *bullet = PlayerGetBullet(player, j);

            for (int k = 0; k < GameGetEnemyCount(game, i); k++) {
                Enemy *enemy = GameGetEnemy(game, i, k);

                if (CheckCollisionRecs(BulletGetBounds(bullet), EnemyGetBounds(enemy))) {
                    BulletHit(bullet);

                    if (EnemyGetStatus(enemy) >= ENEMY_STATUS_DESTROYED) {
                        continue;
                    }

                    EnemyTakeDamage(enemy);

                    if (EnemyGetStatus(enemy) == ENEMY_STATUS_DESTROYED) {
                        PlayerIncrementKillCount(player);
                    }
                }
            }
        }
    }
}

static void ResolveEnemyBulletCollisions(Game *game) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        for (int j = 0; j < GameGetEnemyCount(game, i); j++) {
            Enemy *enemy = GameGetEnemy(game, i, j);

            for (int k = 0; k < EnemyGetBulletCount(enemy); k++) {
                Bullet *bullet = EnemyGetBullet(enemy, k);

                for (int l = 0; l < MAX_PLAYERS; l++) {
                    Player *player = GameGetPlayer(game, l);
                    if (player == NULL) {
                        continue;
                    }

                    if (CheckCollisionRecs(BulletGetBounds(bullet), PlayerGetBounds(player))) {
                        BulletHit(bullet);
                        PlayerTakeDamage(player);
                    }
                }
            }
        }
    }
}

void CollisionUpdate(Game *game) {
    assert(game);

    ResolvePlayerBulletCollisions(game);
    ResolveEnemyBulletCollisions(game);
}
