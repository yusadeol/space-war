#include "controller.h"
#include <raylib.h>

ControllerInput ControllerGetKeyboardInput() {
    return (ControllerInput){
        .left = IsKeyDown(KEY_LEFT),
        .up = IsKeyDown(KEY_UP),
        .right = IsKeyDown(KEY_RIGHT),
        .down = IsKeyDown(KEY_DOWN),
        .shoot = IsKeyPressed(KEY_SPACE),
    };
}

ControllerInput ControllerGetGamepadInput(const int gamepad) {
    return (ControllerInput){
        .left = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_LEFT),
        .up = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_UP),
        .right = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_RIGHT),
        .down = IsGamepadButtonDown(gamepad, GAMEPAD_BUTTON_LEFT_FACE_DOWN),
        .shoot =
            IsGamepadButtonPressed(gamepad, GAMEPAD_BUTTON_RIGHT_FACE_DOWN),
    };
}
