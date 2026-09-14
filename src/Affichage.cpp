//
// Created by thoma on 14/09/2026.
//

#include "Affichage.h"

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
}

