#include "../util.h"

float fsignf(float x) {
    if (x > 0.f) return 1.f;
    if (x < 0.f) return -1.f;
    return 0.f;
}
