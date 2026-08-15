#include "keyboard.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

#include "controller.h"

struct Keyboard {
    Controller controller;
};

static ControllerInput GetInput(const Controller*) {
    return (ControllerInput){
        .left_held = IsKeyDown(KEY_LEFT),
        .up_held = IsKeyDown(KEY_UP),
        .right_held = IsKeyDown(KEY_RIGHT),
        .down_held = IsKeyDown(KEY_DOWN),
        .shoot_pressed = IsKeyPressed(KEY_SPACE),
        .start_pressed = IsKeyPressed(KEY_ENTER),
    };
}

void KeyboardDestroy(Keyboard* keyboard) {
    assert(keyboard);

    free(keyboard);
}

static void Destroy(Controller* controller) {
    Keyboard* keyboard = (Keyboard*)controller;

    KeyboardDestroy(keyboard);
}

Keyboard* KeyboardCreate(void) {
    Keyboard* keyboard = malloc(sizeof(*keyboard));

    if (keyboard == NULL) {
        return NULL;
    }

    *keyboard = (Keyboard){.controller = {.GetInput = GetInput, .Destroy = Destroy}};

    return keyboard;
}
