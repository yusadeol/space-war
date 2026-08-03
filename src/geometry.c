#include "geometry.h"

Vector2 GeometryGetCenterFromRectangle(const Rectangle rectangle) {
    return (Vector2){
        rectangle.x + (rectangle.width / 2.0f),
        rectangle.y + (rectangle.height / 2.0f),
    };
}

Vector2 GeometryGetCenteredPosition(
    const Vector2 reference, const float width, const float height) {
    return (Vector2){
        reference.x - width / 2.0f,
        reference.y - height / 2.0f,
    };
}
