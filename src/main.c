#include <glad/glad.h>  
#include <SDL2/SDL.h>
#include <stdio.h>
#include "engine/render.h"
#include "engine/input.h"
#include "engine/config.h"
#include "engine/util.h"
#include "engine/physics.h"
#include "engine/time.h"
#include "engine/entity.h"

int main(void) {
    Config_State *config_state = config_init();
    SDL_Window *window = render_init(config_state->display_width, config_state->display_height);
    Input_State *input_state = input_init();
    Physics_State *physics_state = physics_init();
    Time_State *time_state = time_init(config_state->framerate);
    Entity_State *entity_state = entity_init();

    SDL_Event e;
    int quit = 0;

    uint32_t player_index = entity_create();
    Entity *player = &entity_state->entities[player_index];
    player->body_id = physics_body_create((vec2){60.f, 500.f}, (vec2){50.f, 50.f}, 0);
    Body *player_body = &physics_state->bodies[player->body_id];

    uint32_t a_index = physics_body_create((vec2){25.f, 200.f}, (vec2){100.f, 5.f}, 1);
    Body *a = &physics_state->bodies[a_index];

    uint32_t b_index = physics_body_create((vec2){config_state->display_width * 0.5f, 20.f}, (vec2){config_state->display_width, 5.f}, 1);
    Body *b = &physics_state->bodies[b_index];

    uint32_t c_index = physics_body_create((vec2){150.f, 200.f}, (vec2){50.f, 250.f}, 1);
    Body *c = &physics_state->bodies[c_index];

    while (!quit) {
        time_update();

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_GetMouseState(&input_state->mouse_x, &input_state->mouse_y);

        input_update(config_state->keybinds);

        glClearColor(0.f, 0.2f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        float xvel = 0.f;
        float yvel = player_body->velocity[1];

        if (input_state->left == KEY_STATE_HELD) {
            xvel -= 300.f;
        }

        if (input_state->right == KEY_STATE_HELD) {
            xvel += 300.f;
        }

        if (input_state->jump == KEY_STATE_PRESSED && player_body->is_grounded) {
            printf("JUMP\n");
            yvel = 700.f;
        }

        player_body->velocity[0] = xvel;
        player_body->velocity[1] = yvel;

        printf("yvel: %2.f\n", player_body->velocity[1]);

        physics_update(time_state->delta);
        printf("yvel: %2.f\n", player_body->velocity[1]);

        render_aabb(&a->aabb, WHITE);
        render_aabb(&b->aabb, WHITE);
        render_aabb(&c->aabb, WHITE);
        render_aabb(&player_body->aabb, GREEN);

        time_late_update();
        SDL_GL_SwapWindow(window);
    }

    return 0;
}
