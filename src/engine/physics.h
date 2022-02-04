#ifndef PHYSICS_H
#define PHYSICS_H

#include <stdlib.h>
#include <linmath.h>
#include <inttypes.h>

#define MAX_BODIES 256
#define GRAVITY -20
#define TERMINAL_VELOCITY -1000

typedef struct physics_state Physics_State;

typedef struct aabb {
	vec2 position;
	vec2 half_size;
} AABB;

typedef struct body {
	AABB aabb;
	vec2 velocity;
	uint8_t is_static;
	uint8_t is_active;
	uint8_t is_grounded;
} Body;

struct physics_state {
	uint32_t body_count;
	uint32_t body_max;
	Body *bodies;
};

uint8_t aabb_intersect_aabb(AABB a, AABB b);
uint8_t aabb_intersect_aabb_moving(AABB a, AABB b, vec2 va, vec2 vb, float *tfirst, float *tlast, float *nx, float *ny);

Physics_State *physics_init(void);
void physics_update(float delta_time);
uint32_t physics_body_create(vec2 position, vec2 size, uint8_t is_static);

#endif
