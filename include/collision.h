#pragma once
#include "game.h"

typedef struct {
    int source_index;

    int *target_indexes;
    int target_index_count;
    int target_index_capacity;

    int *object_indexes;
    int object_index_count;
    int object_index_capacity;
} Collision;

void CollisionUpdate(Game *game);
