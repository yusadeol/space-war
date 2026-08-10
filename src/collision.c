#include "collision.h"

#include "array.h"
#include "bullet.h"
#include "enemy.h"
#include "game.h"
#include "player.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

static void ResolvePlayerBulletCollisions(Game *game) {
    Collision collided_enemies[MAX_PLAYERS] = {};

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
                    Collision *collision = &collided_enemies[i];
                    if (collision->target_indexes == NULL && collision->object_indexes == NULL) {
                        int *enemies = malloc(sizeof(*enemies) * MAX_ENEMIES);
                        if (enemies == NULL) {
                            TraceLog(LOG_ERROR, "Failed to allocate collision enemies array for player %d", i);

                            continue;
                        }

                        int *bullets = malloc(sizeof(*bullets) * MAX_SIMULTANEOUS_BULLETS);
                        if (bullets == NULL) {
                            TraceLog(LOG_ERROR, "Failed to allocate collision bullets array for player %d", i);
                            free(enemies);

                            continue;
                        }

                        *collision = (Collision){
                            .source_index = i,
                            .target_indexes = enemies,
                            .target_index_capacity = MAX_ENEMIES,
                            .object_indexes = bullets,
                            .object_index_capacity = MAX_SIMULTANEOUS_BULLETS,
                        };

                        collision->target_indexes[collision->target_index_count++] = k;
                        collision->object_indexes[collision->object_index_count++] = j;

                        continue;
                    }

                    if ((collision->target_index_count + 1) > collision->target_index_capacity
                        || (collision->object_index_count + 1) > collision->object_index_capacity) {
                        TraceLog(LOG_WARNING,
                            "Collision target/object arrays overflow for player %d (targets: %d/%d, objects: %d/%d)", i,
                            collision->target_index_count, collision->target_index_capacity,
                            collision->object_index_count, collision->object_index_capacity);

                        continue;
                    }

                    collision->target_indexes[collision->target_index_count++] = k;
                    collision->object_indexes[collision->object_index_count++] = j;
                }
            }
        }
    }

    for (int i = 0; i < MAX_PLAYERS; i++) {
        Collision *collision = &collided_enemies[i];
        if (collision->target_indexes == NULL && collision->object_indexes == NULL) {
            continue;
        }

        collision->target_index_count =
            ArrayDeduplicateInteger(collision->target_indexes, collision->target_index_count);
        collision->object_index_count =
            ArrayDeduplicateInteger(collision->object_indexes, collision->object_index_count);

        Player *player = GameGetPlayer(game, collision->source_index);

        if (!PlayerRemoveBullets(player, collision->object_indexes, collision->object_index_count)) {
            TraceLog(LOG_WARNING, "Failed to remove all player %d bullets", collision->source_index);
        }

        int destroyed_enemies[MAX_ENEMIES] = {};
        int destroyed_enemy_count = 0;

        for (int j = 0; j < collision->target_index_count; j++) {
            Enemy *enemy = GameGetEnemy(game, collision->source_index, collision->target_indexes[j]);
            EnemyTakeDamage(enemy);

            if (EnemyGetStatus(enemy) == ENEMY_STATUS_DESTROYED) {
                if ((destroyed_enemy_count + 1) > MAX_ENEMIES) {
                    TraceLog(LOG_WARNING, "Destroyed enemies capacity reached for player %d (%d)",
                        collision->source_index, MAX_ENEMIES);

                    break;
                }

                destroyed_enemies[destroyed_enemy_count++] = collision->target_indexes[j];
            }
        }

        if (destroyed_enemy_count != 0) {
            if (!GameRemoveEnemies(game, collision->source_index, destroyed_enemies, destroyed_enemy_count)) {
                TraceLog(LOG_WARNING, "Failed to remove all destroyed enemies for player %d", collision->source_index);
            }
            PlayerIncrementKillCountByAmount(player, destroyed_enemy_count);
        }

        free(collision->target_indexes);
        free(collision->object_indexes);
    }
}

static void ResolveEnemyBulletCollisions(Game *game) {
    Collision collided_players[MAX_PLAYERS][MAX_ENEMIES] = {};

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
                        Collision *collision = &collided_players[i][j];
                        if (collision->target_indexes == NULL && collision->object_indexes == NULL) {
                            int *players = malloc(sizeof(*players) * MAX_PLAYERS);
                            if (players == NULL) {
                                TraceLog(LOG_ERROR, "Failed to allocate collision players array for player %d enemy %d",
                                    i, j);

                                continue;
                            }

                            int *bullets = malloc(sizeof(*bullets) * MAX_SIMULTANEOUS_BULLETS);
                            if (bullets == NULL) {
                                TraceLog(LOG_ERROR, "Failed to allocate collision bullets array for player %d enemy %d",
                                    i, j);
                                free(players);

                                continue;
                            }

                            *collision = (Collision){
                                .source_index = j,
                                .target_indexes = players,
                                .target_index_capacity = MAX_PLAYERS,
                                .object_indexes = bullets,
                                .object_index_capacity = MAX_SIMULTANEOUS_BULLETS,
                            };

                            collision->target_indexes[collision->target_index_count++] = l;
                            collision->object_indexes[collision->object_index_count++] = k;

                            continue;
                        }

                        if ((collision->target_index_count + 1) > collision->target_index_capacity
                            || (collision->object_index_count + 1) > collision->object_index_capacity) {
                            TraceLog(LOG_WARNING,
                                "Collision target/object arrays overflow for player %d enemy %d (targets: %d/%d, "
                                "objects: %d/%d)",
                                i, j, collision->target_index_count, collision->target_index_capacity,
                                collision->object_index_count, collision->object_index_capacity);

                            continue;
                        }

                        collision->target_indexes[collision->target_index_count++] = l;
                        collision->object_indexes[collision->object_index_count++] = k;
                    }
                }
            }
        }
    }

    int destroyed_players[MAX_PLAYERS] = {};
    int destroyed_player_count = 0;

    for (int i = 0; i < MAX_PLAYERS; i++) {
        for (int j = 0; j < MAX_ENEMIES; j++) {
            Collision *collision = &collided_players[i][j];
            if (collision->target_indexes == NULL && collision->object_indexes == NULL) {
                continue;
            }

            collision->target_index_count =
                ArrayDeduplicateInteger(collision->target_indexes, collision->target_index_count);
            collision->object_index_count =
                ArrayDeduplicateInteger(collision->object_indexes, collision->object_index_count);

            Enemy *enemy = GameGetEnemy(game, i, collision->source_index);

            if (!EnemyRemoveBullets(enemy, collision->object_indexes, collision->object_index_count)) {
                TraceLog(LOG_WARNING, "Failed to remove all enemy %d bullets for player %d", collision->source_index,
                    i);
            }

            for (int k = 0; k < collision->target_index_count; k++) {
                if ((destroyed_player_count + 1) > MAX_PLAYERS) {
                    break;
                }

                bool exists = false;

                for (int l = 0; l < destroyed_player_count; l++) {
                    if (collision->target_indexes[k] == destroyed_players[l]) {
                        exists = true;
                    }
                }

                if (!exists) {
                    destroyed_players[destroyed_player_count++] = collision->target_indexes[k];
                }
            }

            free(collision->target_indexes);
            free(collision->object_indexes);
        }
    }

    if (destroyed_player_count != 0) {
        destroyed_player_count = ArrayDeduplicateInteger(destroyed_players, destroyed_player_count);
        if (!GameRemovePlayers(game, destroyed_players, destroyed_player_count)) {
            TraceLog(LOG_WARNING, "Failed to remove all destroyed players");
        }
    }
}

void CollisionUpdate(Game *game) {
    assert(game);

    ResolvePlayerBulletCollisions(game);
    ResolveEnemyBulletCollisions(game);
}
