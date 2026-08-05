#pragma once

typedef struct {
    bool left;
    bool up;
    bool right;
    bool down;
    bool shoot;
} ControllerInput;

ControllerInput ControllerGetKeyboardInput();
ControllerInput ControllerGetGamepadInput(const int gamepad);
