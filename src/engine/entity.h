#ifndef ENTITY_H
#define ENTITY_H

#include <stdlib.h>
#include <linmath.h>

#define MAX_ENTITIES 256

typedef struct entity {
	uint32_t body_id;
	uint8_t is_active;
} Entity;

typedef struct entity_state {
	Entity *entities;
	uint32_t entity_count;
	uint32_t entity_max;
} Entity_State;

Entity_State *entity_init(void);
uint32_t entity_create(void);

#endif
