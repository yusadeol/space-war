#pragma once
#include <raylib.h>

Vector2 GeometryGetCenterFromRect(const Rectangle rec);
Vector2 GeometryGetTopLeftFromCenter(
    const Vector2 center, const float width, const float height);
