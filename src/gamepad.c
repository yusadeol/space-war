#include "gamepad.h"

#include "controller.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Gamepad {
    Controller controller;
    int gamepad_index;
};

static ControllerInput GetInput(const Controller *controller) {
    const Gamepad *gamepad = (const Gamepad *)controller;

    return (ControllerInput){
        .left = IsGamepadButtonDown(gamepad->gamepad_index, GAMEPAD_BUTTON_LEFT_FACE_LEFT),
        .up = IsGamepadButtonDown(gamepad->gamepad_index, GAMEPAD_BUTTON_LEFT_FACE_UP),
        .right = IsGamepadButtonDown(gamepad->gamepad_index, GAMEPAD_BUTTON_LEFT_FACE_RIGHT),
        .down = IsGamepadButtonDown(gamepad->gamepad_index, GAMEPAD_BUTTON_LEFT_FACE_DOWN),
        .shoot = IsGamepadButtonPressed(gamepad->gamepad_index, GAMEPAD_BUTTON_RIGHT_FACE_DOWN),
        .start = IsGamepadButtonPressed(gamepad->gamepad_index, GAMEPAD_BUTTON_MIDDLE_RIGHT),
    };
}

void GamepadDestroy(Gamepad *gamepad) {
    assert(gamepad);

    free(gamepad);
}

static void Destroy(Controller *controller) {
    Gamepad *gamepad = (Gamepad *)controller;

    GamepadDestroy(gamepad);
}

Gamepad *GamepadCreate(const int gamepad_index) {
    if (!IsGamepadAvailable(gamepad_index)) {
        return NULL;
    }

    Gamepad *gamepad = malloc(sizeof(*gamepad));

    if (gamepad == NULL) {
        return NULL;
    }

    *gamepad = (Gamepad){
        .controller = {.GetInput = GetInput, .Destroy = Destroy},
        .gamepad_index = gamepad_index,
    };

    return gamepad;
}
