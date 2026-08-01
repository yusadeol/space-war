#include "geometry.h"

Vector2 RectangleCenter(Rectangle rec) {
    return (Vector2){
        rec.x + (rec.width / 2.0f),
        rec.y + (rec.height / 2.0f),
    };
}
