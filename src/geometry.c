#include "geometry.h"

Vector2 CenterFromRectangle(const Rectangle rec) {
    return (Vector2){
        rec.x + (rec.width / 2.0f),
        rec.y + (rec.height / 2.0f),
    };
}

Vector2 PositionFromCenter(
    const Vector2 center, const float width, const float height) {
    return (Vector2){
        center.x - width / 2.0f,
        center.y - height / 2.0f,
    };
}
