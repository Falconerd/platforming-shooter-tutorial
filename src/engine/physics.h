#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdlib.h>
#include <linmath.h>

typedef struct aabb {
    vec2 position;
    vec2 half_size;
} AABB;

uint8_t aabb_intersect_aabb(AABB a, AABB b);
uint8_t aabb_intersect_aabb_moving(AABB a, AABB b, vec2 va, vec2 vb, float *tfirst, float *tlast);

#endif
