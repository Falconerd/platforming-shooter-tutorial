#include <stdio.h>
#include <inttypes.h>
#include "../entity.h"

static Entity_State entity_state;

Entity_State *entity_init(void) {
	entity_state.entities = malloc(MAX_ENTITIES * sizeof(*entity_state.entities));
	return &entity_state;
}

static uint32_t next_entity_index() {
	uint32_t index = 0xdeadbeef;

	for (uint32_t i = 0; i < MAX_ENTITIES; ++i) {
		if (!entity_state.entities[i].is_active) {
			index = i;
			break;
		}
	}

	if (index == 0xdeadbeef) {
		printf("No space for new entities! Exiting.\n");
		exit(1);
	}

	return index;
}

uint32_t entity_create() {
	uint32_t index = next_entity_index();
	Entity *entity = &entity_state.entities[index];

	memset(entity, 0, sizeof(*entity));

	return index;
}
