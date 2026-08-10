#pragma once

typedef struct {
    bool left;
    bool up;
    bool right;
    bool down;
    bool shoot;
    bool start;
} ControllerInput;

typedef struct Controller Controller;

struct Controller {
    ControllerInput (*GetInput)(const Controller *controller);
    void (*Destroy)(Controller *controller);
};
