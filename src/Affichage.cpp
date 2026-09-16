//
// Created by thoma on 14/09/2026.
//

#include "Affichage.h"

#include <cmath>

#include "background_sprite.h"
#include "background_sprite_path.h"

Affichage::Affichage() {
    // load map.bin
}

void Affichage::process() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
    SDL_RenderClear(renderer);

    //get window size
    SDL_GetWindowSize(window, &window_width, &window_height);

    // show background tiles
    //

    // show sprites
    //

    // show player
    // debug player for now
    // player should be in like "engine->player" or some shit like that

    // debug/test
    SDL_FRect src = {
        16.0f*0, // frame in spritesheet
        0,
        16,
        16
    };
    float scale_x = (float)window_width  / REFERENCE_WINDOW_WIDTH;
    float scale_y = (float)window_height / REFERENCE_WINDOW_HEIGHT;

    float scale = fminf(scale_x, scale_y);

    SDL_FRect dst = {
        0.0f,
        0.0f,
        16.0f * 4.0f * scale,
        16.0f * 4.0f * scale
    };

    SDL_RenderTexture(renderer, test, &src, &dst);

    SDL_RenderPresent(renderer);
}

