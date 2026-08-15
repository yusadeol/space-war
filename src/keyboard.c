#include "keyboard.h"

#include "controller.h"

#include <assert.h>
#include <raylib.h>
#include <stdlib.h>

struct Keyboard {
    Controller controller;
};

static ControllerInput GetInput(const Controller *) {
    return (ControllerInput){
        .left = IsKeyDown(KEY_LEFT),
        .up = IsKeyDown(KEY_UP),
        .right = IsKeyDown(KEY_RIGHT),
        .down = IsKeyDown(KEY_DOWN),
        .shoot = IsKeyPressed(KEY_SPACE),
        .start = IsKeyPressed(KEY_ENTER),
    };
}

void KeyboardDestroy(Keyboard *keyboard) {
    assert(keyboard);

    free(keyboard);
}

static void Destroy(Controller *controller) {
    Keyboard *keyboard = (Keyboard *)controller;

    KeyboardDestroy(keyboard);
}

Keyboard *KeyboardCreate(void) {
    Keyboard *keyboard = malloc(sizeof(*keyboard));

    if (keyboard == NULL) {
        return NULL;
    }

    *keyboard = (Keyboard){.controller = {.GetInput = GetInput, .Destroy = Destroy}};

    return keyboard;
}
