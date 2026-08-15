#pragma once

typedef struct {
    bool left_held;
    bool up_held;
    bool right_held;
    bool down_held;
    bool shoot_pressed;
    bool start_pressed;
} ControllerInput;

typedef struct Controller Controller;

struct Controller {
    ControllerInput (*GetInput)(const Controller *controller);
    void (*Destroy)(Controller *controller);
};
