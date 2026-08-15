#pragma once

typedef struct Keyboard Keyboard;

void KeyboardDestroy(Keyboard *keyboard);
Keyboard *KeyboardCreate(void);
