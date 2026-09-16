//
// Created by thoma on 14/09/2026.
//

#include "Affichage.h"
#include "background_sprite.h"
#include "background_sprite_path.h"

Affichage::Affichage() {
    //
}

void Affichage::process() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running = false;
        }
    }
    // show background tiles
    //

    // show sprites
    //
}

