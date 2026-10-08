//
// Created by thoma on 14/09/2026.
//

#ifndef PROJET_SAE_AFFICHAGE_H
#define PROJET_SAE_AFFICHAGE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <time.h>

#include "background_sprite.h"
#include "background_sprite_path.h"

#include "Engine.h"

#define REFERENCE_WINDOW_HEIGHT 720
#define REFERENCE_WINDOW_WIDTH 1080
#define MAP_WIDTH 768
#define MAP_HEIGHT 768

class Affichage {
public:
    Affichage(Engine& e);
    ~Affichage();
    //methods
    void process();
    //vars
    bool running=true;
private:
    SDL_Window *window = SDL_CreateWindow("Jeu", 1080, 720, SDL_WINDOW_RESIZABLE);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

    Engine& engine;

    void draw_background();
    void draw_sprites();

    //just declaring those so I can change them, instead of making new variable each frame
    int window_width=1080;
    int window_height=720;

    // test texture:
    SDL_Surface *img_test = IMG_Load(background_path[MISSING]);
    SDL_Texture *test = SDL_CreateTextureFromSurface(renderer, img_test);

    // textures
    SDL_Texture *tiles_textures[256]; // change later
    SDL_Texture *sprites_textures[256]; // change later

    //map
    uint8_t map[MAP_HEIGHT][MAP_WIDTH] {0};

    bool background_animation_state = false; // can only do 2state for now
};


#endif //PROJET_SAE_AFFICHAGE_H
