#include <string.h>
#include <stdio.h>
#include "../io.h"
#include "../config.h"
#include "config_internal.h"
#include "../input.h"

static char *config_get_value(char *string) {
    char *line = strdup(string);
    char *curr = line;

    while (*curr != '\n' && *curr != 0) {
        ++curr;
    }
    *curr = 0;

    char *delimeter = strstr(line, "= ") ? "= " : "=";
    strtok(line, delimeter);
    char *value = strtok(NULL, delimeter);

    return value;
}

static char *find(char *haystack, char *needle) {
    char *result = strstr(haystack, needle);
    if (!result) {
        printf("Could not find config value '%s'. Try deleting config.ini to regenerate default settings. Exiting.\n", needle);
        exit(1);
    }
    return result;
}

static void load_controls(char *config_buffer) {
    char *left = find(config_buffer, "left");
    char *right = find(config_buffer, "right");
    char *jump = find(config_buffer, "jump");
    char *shoot = find(config_buffer, "shoot");

    config_key_bind(INPUT_KEY_LEFT, config_get_value(left));
    config_key_bind(INPUT_KEY_RIGHT, config_get_value(right));
    config_key_bind(INPUT_KEY_JUMP, config_get_value(jump));
    config_key_bind(INPUT_KEY_SHOOT, config_get_value(shoot));
}

static void load_display(Config_State *config_state, char *config_buffer) {
    char *width = find(config_buffer, "width");
    char *height = find(config_buffer, "height");
    char *framerate = find(config_buffer, "framerate");

    config_state->display_width = (float)atof(config_get_value(width));
    config_state->display_height = (float)atof(config_get_value(height));
    config_state->framerate = (float)atof(config_get_value(framerate));
}

int config_init_load(Config_State *config_state) {
    char *config_buffer = io_file_read("./config.ini");
    if (!config_buffer) {
        return 1;
    }

    load_controls(config_buffer);
    load_display(config_state, config_buffer);

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
