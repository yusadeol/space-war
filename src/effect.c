#include "effect.h"

Color EffectBlinkColor(float rate) {
    int phase = (int)(GetTime() * rate);
    return phase % 2 ? WHITE : BLANK;
}
