#pragma once
#include <raylib.h>

constexpr int WORLD_BORDER = 5;
constexpr Color WORLD_BACKGROUND_COLOR = {0, 0, 0, 255};

typedef struct World World;

World *WorldCreate(const int window_width, const int window_height, const int border, Color background_color);
void WorldDestroy(World *world);

int WorldGetWidth(const World *world);
int WorldGetHeight(const World *world);
int WorldGetBorder(const World *world);
Color WorldGetBackgroundColor(const World *world);

Vector2 WorldResolveBoundaries(const World *world, const Rectangle bounds);

bool WorldIsOutOfBounds(const World *world, const Rectangle bounds);
