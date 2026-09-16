//
// Created by thoma on 14/09/2026.
//

#ifndef PROJET_SAE_AFFICHAGE_H
#define PROJET_SAE_AFFICHAGE_H

#include <SDL3/SDL.h>

#include "background_sprite.h"

class Affichage {
public:
    Affichage();
    ~Affichage();
    //methods
    void process();
    //vars
    bool running=true;
private:
    SDL_Window *window = SDL_CreateWindow("Jeu", 1080, 720, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
};


#endif //PROJET_SAE_AFFICHAGE_H
