#include <string.h>
#include <stdio.h>
#include "../io.h"
#include "../config.h"
#include "config_internal.h"
#include "../input.h"
#include "../util.h"

static char *config_buffer;
static char *tmp_buffer;

static char *config_get_value(const char *string) {
	char *line = strstr(config_buffer, string);
	if (!line) {
		printf("Could not find config value '%s'. Try deleting config.ini to regenerate default settings. Exiting.\n", string);
		exit(1);
	}

	memcpy(tmp_buffer, line, 99);

	char *curr = tmp_buffer;

	while (*curr != '\n' && *curr != 0) {
		++curr;
	}
	*curr = 0;

	char *delimeter = strstr(tmp_buffer, "= ") ? "= " : "=";
	strtok(tmp_buffer, delimeter);
	char *value = strtok(NULL, delimeter);
	size_t len = strlen(value);

	memcpy(tmp_buffer, value, len + 1);
	tmp_buffer[len] = 0;

	return tmp_buffer;
}

static void load_controls() {
	config_key_bind(INPUT_KEY_LEFT, config_get_value("left"));
	config_key_bind(INPUT_KEY_RIGHT, config_get_value("right"));
	config_key_bind(INPUT_KEY_JUMP, config_get_value("jump"));
	config_key_bind(INPUT_KEY_SHOOT, config_get_value("shoot"));
}

static void load_display(Config_State *config_state) {
	config_state->display_width = (float)atof(config_get_value("width"));
	config_state->display_height = (float)atof(config_get_value("height"));
	config_state->framerate = (float)atof(config_get_value("framerate"));
}

int config_init_load(Config_State *config_state) {
	config_buffer = io_file_read("./config.ini");
	if (!config_buffer) {
		return 1;
	}

	tmp_buffer = malloc(100);
	if (!tmp_buffer) {
		return 1;
	}

	load_controls();
	load_display(config_state);

	free(config_buffer);
	free(tmp_buffer);

	return 0;
}

void config_init_create_default(void) {
	char *default_config_file = "[controls]\n"
		"left = A\n"
		"right = D\n"
		"jump = Space\n"
		"shoot = H\n"
		"\n"
		"[display]\n"
		"width = 800\n"
		"height = 600\n"
		"framerate = 60\n";

	io_file_write(default_config_file, strlen(default_config_file), "./config.ini");
}
