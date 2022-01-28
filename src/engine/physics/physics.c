#include "../physics.h"
#include "physics_internal.h"

// NOTE: DELETE ME
#include <stdio.h>

static Physics_State physics_state;

Physics_State *physics_init() {
    physics_init_bodies(&physics_state);

    return &physics_state;
}

uint8_t aabb_intersect_aabb(AABB a, AABB b) {
    for (int i = 0; i < 2; ++i) {
        if (a.position[i] + a.half_size[i] < b.position[i] - b.half_size[i] || a.position[i] - a.half_size[i] > b.position[i] + b.half_size[i]) {
            return 0;
        }
    }
    return 1;
}

uint8_t aabb_intersect_aabb_moving(AABB a, AABB b, vec2 va, vec2 vb, float *tfirst, float *tlast) {
    if (aabb_intersect_aabb(a, b)) {
        *tfirst = *tlast = 0.f;
        return 1;
    }

    vec2 v;
    vec2_sub(v, vb, va);

    *tfirst = 0.f;
    *tlast = 1.f;

    vec2 amin, amax, bmin, bmax;
    vec2_sub(amin, a.position, a.half_size);
    vec2_sub(bmin, b.position, b.half_size);
    vec2_add(amax, a.position, a.half_size);
    vec2_add(bmax, b.position, b.half_size);

    for (uint8_t i = 0; i < 2; ++i) {
        if (v[i] < 0.f) {
            if (bmax[i] < amin[i]) return 0;
            if (amax[i] < bmin[i]) *tfirst = fmaxf((amax[i] - bmin[i]) / v[i], *tfirst);
            if (bmax[i] > amin[i]) *tlast = fminf((amin[i] - bmax[i]) / v[i], *tlast);
        }

        if (v[i] > 0.f) {
            if (bmin[i] > amax[i]) return 0;
            if (bmax[i] < amin[i]) *tfirst = fmaxf((amin[i] - bmax[i]) / v[i], *tfirst);
            if (amax[i] > bmin[i]) *tlast = fminf((amax[i] - bmin[i]) / v[i], *tlast);
        }

        if (*tfirst > *tlast) return 0;
    }

    return 1;
}

void physics_update(float delta_time) {
    for (uint32_t i = 0; i <= physics_state.body_max; ++i) {
        Body *a = &physics_state.bodies[i];

        if (!a->is_active) continue;

        if (!a->is_static) {
            a->velocity[1] += GRAVITY;
            if (a->velocity[1] < TERMINAL_VELOCITY)
                a->velocity[1] = TERMINAL_VELOCITY;

            uint8_t hit;

            for (uint32_t j = 0; j <= physics_state.body_max; ++j) {
                if (i == j) continue;

                Body *b = &physics_state.bodies[j];

                if (!b->is_active) continue;

                float tfirst, tlast;
                vec2 va, vb;
                vec2_scale(va, a->velocity, delta_time);
                vec2_scale(vb, b->velocity, delta_time);

                hit = aabb_intersect_aabb_moving(a->aabb, b->aabb, va, vb, &tfirst, &tlast);

                if (hit && b->is_static) {
                    printf("%u hit %u\n", i, j);
                    a->aabb.position[0] = a->aabb.position[0] + va[0] * tfirst;
                    a->aabb.position[1] = a->aabb.position[1] + va[1] * tfirst;
                }
            }

            if (!hit) {
                a->aabb.position[0] += a->velocity[0] * delta_time;
                a->aabb.position[1] += a->velocity[1] * delta_time;
            }
        }
    }
}

static uint32_t next_body_index() {
    uint32_t index = 0xdeadbeef;

    for (uint32_t i = 0; i < MAX_BODIES; ++i) {
        if (!physics_state.bodies[i].is_active) {
            index = i;
            break;
        }
    }

    if (index == 0xdeadbeef) {
        printf("No space for new bodies! Exiting.\n");
        exit(1);
    }

    return index;
}

uint32_t physics_body_create(vec2 position, vec2 size, uint8_t is_static) {
    uint32_t index = next_body_index();

    Body *body = &physics_state.bodies[index];
    body->aabb.position[0] = position[0];
    body->aabb.position[1] = position[1];
    body->aabb.half_size[0] = size[0] * 0.5f;
    body->aabb.half_size[1] = size[1] * 0.5f;
    body->is_active = 1;
    body->is_static = is_static;
    body->velocity[0] = 0;
    body->velocity[1] = 0;

    if (index > physics_state.body_max) {
        physics_state.body_max = index;
    }

    ++physics_state.body_count;

    return index;
}
