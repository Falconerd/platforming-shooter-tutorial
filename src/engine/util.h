#ifndef UTIL_H
#define UTIL_H

#include <stdlib.h>

float fsignf(float x);
float fminf(float a, float b);
float fmaxf(float a, float b);

#define WHITE (vec4){1.f, 1.f, 1.f, 1.f}
#define RED (vec4){1.f, 0.f, 0.f, 1.f}
#define GREEN (vec4){0.f, 1.f, 0.f, 1.f}
#define BLUE (vec4){0.f, 0.f, 1.f, 1.f}
#define YELLOW (vec4){1.f, 1.f, 0.f, 1.f}
#define PINK (vec4){1.f, 0.f, 1.f, 1.f}

#endif
