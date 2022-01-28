#include <glad/glad.h>  
#include <SDL2/SDL.h>
#include <stdio.h>
#include "engine/render.h"
#include "engine/input.h"
#include "engine/config.h"
#include "engine/util.h"
#include "engine/physics.h"
#include "engine/time.h"

int main(void) {
    Config_State *config_state = config_init();
    SDL_Window *window = render_init(config_state->display_width, config_state->display_height);
    Input_State *input_state = input_init();
    Physics_State *physics_state = physics_init();
    Time_State *time_state = time_init(config_state->framerate);

    SDL_Event e;
    int quit = 0;

    uint32_t a_index = physics_body_create((vec2){50.f, 500.f}, (vec2){50.f, 50.f}, 0);
    Body *a = &physics_state->bodies[a_index];
    uint32_t b_index = physics_body_create((vec2){config_state->display_width * 0.5f, 20.f}, (vec2){config_state->display_width, 10.f}, 1);
    Body *b = &physics_state->bodies[b_index];

    while (!quit) {
        time_update();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_GetMouseState(&input_state->mouse_x, &input_state->mouse_y);

        input_update(config_state->keybinds);

        physics_update(time_state->delta);

        glClearColor(0.f, 0.2f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_aabb(&a->aabb, GREEN);
        render_aabb(&b->aabb, WHITE);

        time_late_update();

        SDL_GL_SwapWindow(window);
    }

    return 0;
}
