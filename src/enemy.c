#include "enemy.h"
#include "asset.h"
#include "bullet.h"
#include <raylib.h>
#include <stdlib.h>

struct Enemy {
    Texture2D texture;
    Vector2 position;
    Bullet *bullets[MAX_SIMULTANEOUS_BULLETS];
    int bullet_count;
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

    *enemy = (Enemy){.position = {99999.0f, 0}, .texture = EnemyTexture(type)};

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

TextureSize EnemyGetSize(const Enemy *enemy) {
    return (TextureSize){enemy->texture.width, enemy->texture.height};
}

Bullet *EnemyGetBullet(Enemy *enemy, const int index) {
    return enemy->bullets[index];
}

int EnemyGetBulletCount(const Enemy *enemy) {
    return enemy->bullet_count;
}

void EnemyUpdate(Enemy *enemy, float delta) {
    float move_step = ENEMY_SPEED * delta;

    enemy->position.y += move_step;
}

void EnemyDraw(const Enemy *enemy) {

    Rectangle source = {.width = -enemy->texture.width,
                        .height = enemy->texture.height};
    Rectangle destination = {enemy->position.x, enemy->position.y,
                             EnemyGetWidth(enemy), EnemyGetHeight(enemy)};

    DrawTexturePro(enemy->texture, source, destination, (Vector2){}, 0, WHITE);
}

bool EnemyShot(Enemy *enemy) {
    if ((enemy->bullet_count + 1) > MAX_SIMULTANEOUS_BULLETS) {
        return false;
    }

    Vector2 position = {enemy->position.x + (EnemyGetWidth(enemy) / 2),
                        enemy->position.y + (EnemyGetHeight(enemy) / 2)};

    enemy->bullets[enemy->bullet_count++] =
        BulletCreate(BULLET_TYPE_PULSE, position);

    return true;
}

void EnemySpliceBullet(Enemy *enemy, const int index) {
    Bullet *bullet = enemy->bullets[index];
    BulletDestroy(bullet);

    for (int i = index; i < enemy->bullet_count - 1; i++) {
        enemy->bullets[i] = enemy->bullets[i + 1];
    }

    enemy->bullet_count--;
}
