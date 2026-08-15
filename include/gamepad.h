#pragma once

typedef struct Gamepad Gamepad;

void GamepadDestroy(Gamepad* gamepad);
Gamepad* GamepadCreate(const int gamepad_index);
