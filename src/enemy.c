#include "enemy.h"
#include "asset.h"
#include "bullet.h"
#include "game.h"
#include "geometry.h"
#include <math.h>
#include <raylib.h>
#include <stdlib.h>

struct Enemy {
    Texture2D texture;
    Vector2 position;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
    float shot_cooldown;
};

static Texture2D EnemyTexture(const EnemyType type) {
    switch (type) {
    case ENEMY_TYPE_SPECTRA:
        return *AssetGetTexture(TEXTURE_SPACESHIP_SPECTRA);
    }

    return (Texture2D){};
}

Enemy *EnemyCreate(const EnemyType type) {
    Enemy *enemy = malloc(sizeof(*enemy));

    if (enemy == NULL) {
        return NULL;
    }

    *enemy = (Enemy){.texture = EnemyTexture(type),
                     .shot_cooldown = ENEMY_SHOT_COOLDOWN};

    Rectangle bounds = EnemyGetBounds(enemy);

    enemy->position = (Vector2){
        GAME_WINDOW_WIDTH,
        GetRandomValue(
            WORLD_BORDER, GAME_WINDOW_HEIGHT - (bounds.height + WORLD_BORDER))};

    return enemy;
}

void EnemyDestroy(Enemy *enemy) {
    for (int i = 0; i < enemy->bullet_count; i++) {
        Bullet *bullet = enemy->bullets[i];
        BulletDestroy(bullet);
    }

    free(enemy);
}

float EnemyGetWidth(const Enemy *enemy) {
    return enemy->texture.width * ENEMY_SCALE;
}

float EnemyGetHeight(const Enemy *enemy) {
    return enemy->texture.height * ENEMY_SCALE;
}

Vector2 *EnemyGetPosition(Enemy *enemy) {
    return &enemy->position;
}

Rectangle EnemyGetBounds(const Enemy *enemy) {
    return (Rectangle){
        .x = enemy->position.x,
        .y = enemy->position.y,
        .width = EnemyGetWidth(enemy),
        .height = EnemyGetHeight(enemy),
    };
}

Vector2 EnemyGetCenterPosition(const Enemy *enemy) {
    return GeometryGetCenterFromRectangle(EnemyGetBounds(enemy));
}

Bullet *EnemyGetBullet(Enemy *enemy, const int index) {
    return enemy->bullets[index];
}

int EnemyGetBulletCount(const Enemy *enemy) {
    return enemy->bullet_count;
}

void EnemyUpdate(
    Enemy *enemy, const Vector2 player_position,
    const Vector2 player_center_position, const float delta) {
    float move_step = ENEMY_SPEED * delta;
    Rectangle bounds = EnemyGetBounds(enemy);
    Vector2 enemy_center_position = EnemyGetCenterPosition(enemy);
    float player_distance_y =
        fabsf(player_center_position.y - enemy_center_position.y);

    enemy->shot_cooldown -= delta;

    if (enemy->position.x >
        GAME_WINDOW_WIDTH - ((bounds.width + WORLD_BORDER) * 2)) {
        enemy->position.x -= move_step;
    }

    if (enemy->position.y > player_position.y) {
        enemy->position.y -= move_step;
    }

    if (enemy->position.y < player_position.y) {
        enemy->position.y += move_step;
    }

    if (player_distance_y <= ENEMY_ATTACK_DISTANCE &&
        enemy->shot_cooldown <= 0.0f) {
        EnemyShoot(enemy);

        enemy->shot_cooldown = ENEMY_SHOT_COOLDOWN;
    }
}

void EnemyDraw(const Enemy *enemy) {

    Rectangle source = {.width = -enemy->texture.width,
                        .height = enemy->texture.height};
    Rectangle destination = {enemy->position.x, enemy->position.y,
                             EnemyGetWidth(enemy), EnemyGetHeight(enemy)};

    DrawTexturePro(enemy->texture, source, destination, (Vector2){}, 0, WHITE);
}

bool EnemyShoot(Enemy *enemy) {
    if ((enemy->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return false;
    }

    enemy->bullets[enemy->bullet_count++] = BulletCreate(
        BULLET_TYPE_BOLT, BULLET_DIRECTION_LEFT, EnemyGetBounds(enemy));

    return true;
}

void EnemyRemoveBullet(Enemy *enemy, const int index) {
    Bullet *bullet = enemy->bullets[index];
    BulletDestroy(bullet);

    for (int i = index; i < enemy->bullet_count - 1; i++) {
        enemy->bullets[i] = enemy->bullets[i + 1];
    }

    enemy->bullet_count--;
}
