#include <glad/glad.h>  
#include <SDL2/SDL.h>
#include <stdio.h>
#include "render/render.h"
#include "input/input.h"
#include "config/config.h"
#include "util/util.h"

typedef struct aabb {
    vec2 position;
    vec2 half_size;
} AABB;

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

int main(void) {
    Config_State *config_state = config_init();
    SDL_Window *window = render_init(config_state->display_width, config_state->display_height);
    Input_State *input_state = input_init();

    AABB a = {{50.f, 120.f}, {50.f, 50.f}};
    AABB b = {{550.f, 150.f}, {50.f, 50.f}};
    vec2 va = {500.f, 0.f};
    vec2 vb = {-200.f, 0.f};

    SDL_Event e;
    int quit = 0;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            // if (e.type == SDL_MOUSEBUTTONDOWN) {
            // }
        }

        SDL_GetMouseState(&input_state->mouse_x, &input_state->mouse_y);

        input_update(config_state->keybinds);

        glClearColor(0.f, 0.2f, 0.3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        render_aabb(&a, GREEN);
        render_aabb(&b, WHITE);

        float tfirst, tlast;
        if (aabb_intersect_aabb_moving(a, b, va, vb, &tfirst, &tlast)) {
            AABB collision_aabb;
            collision_aabb.position[0] = a.position[0] + va[0] * tfirst;
            collision_aabb.position[1] = a.position[1] + va[1] * tfirst;
            collision_aabb.half_size[0] = a.half_size[0];
            collision_aabb.half_size[1] = a.half_size[1];

            render_aabb(&collision_aabb, YELLOW);
        }

        if (aabb_intersect_aabb_moving(b, a, vb, va, &tfirst, &tlast)) {
            AABB collision_aabb;
            collision_aabb.position[0] = b.position[0] + vb[0] * tfirst;
            collision_aabb.position[1] = b.position[1] + vb[1] * tfirst;
            collision_aabb.half_size[0] = b.half_size[0];
            collision_aabb.half_size[1] = b.half_size[1];

            render_aabb(&collision_aabb, PINK);
        }

        vec2 next_a_pos;
        vec2_add(next_a_pos, a.position, va);
        render_line_segment(a.position, next_a_pos, WHITE);

        vec2 next_b_pos;
        vec2_add(next_b_pos, b.position, vb);
        render_line_segment(b.position, next_b_pos, WHITE);

        SDL_GL_SwapWindow(window);
    }

    return 0;
}
