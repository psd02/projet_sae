//
// Created by thoma on 14/09/2026.
//

#include "Affichage.h"

#include <cmath>
#include <fstream>
#include <iosfwd>
#include <iostream>

#include "background_sprite.h"
#include "background_sprite_path.h"

Affichage::Affichage() {
    // load map.bin
    std::ifstream file("assets/map.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file assets/map.bin\n";
    } else {
        uint8_t value;
        while (file.get(reinterpret_cast<char&>(value))) {
            // Do something with each byte
            uint16_t y = value/MAP_HEIGHT;
            uint16_t x = value%MAP_WIDTH;
            map[y][x] = value;
            //std::cout << static_cast<int>(value) << '\n';
        }
    }

    // load textures
    for (int i=0; i<3; i++) { // only 3 tiles for now
        // load texture
        SDL_Surface* surface = IMG_Load(background_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << background_path[i] << ", defaulting to missing.png\n";
            //missing.png should be saved inside the code tbh
            surface = IMG_Load(background_path[0]);
            tiles_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        } else {
            tiles_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        }
    }

    for (int i=0; i<3; i++) { // only 3 sprites for now
        // load texture
        SDL_Surface* surface = IMG_Load(sprite_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << sprite_path[i] << ", defaulting to missing.png\n";
            //missing.png should be saved inside the code tbh
            surface = IMG_Load(sprite_path[0]);
            sprites_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        } else {
            sprites_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        }
    }
}

void Affichage::process() {
    SDL_RenderClear(renderer);

    //get window size
    SDL_GetWindowSize(window, &window_width, &window_height);

    float scale_x = (float)window_width  / REFERENCE_WINDOW_WIDTH;
    float scale_y = (float)window_height / REFERENCE_WINDOW_HEIGHT;
    float scale = fminf(scale_x, scale_y); // chose the smallest number between 2 floats

    //debug while waiting for real player position
    int16_t playerx = 2;
    int16_t playery = 2;

    // show background tiles
    int16_t cameraX = playerx * (16*4*scale) - window_width / 2;
    int16_t cameraY = playery * (16*4*scale) - window_height / 2;
    for (int y=playery-16; y<0+5; y++) { // 0 is player Y
        for (int x=playerx-16; x<0+5; x++) { // 0 is player X
            //
            if (x<0 || x>=MAP_WIDTH || y<0 || y>=MAP_HEIGHT) {
                continue;
            }
            SDL_FRect dst = {
                (float)x*16*4*scale - (float)cameraX,
                (float)y*16*4*scale - (float)cameraY,
                16.0f * 4.0f * scale,
                16.0f * 4.0f * scale
            };

            SDL_FRect src = {
                16.0f*0, // frame in spritesheet
                0,
                15,
                15
            };

            SDL_RenderTexture(renderer, tiles_textures[map[y][x]], &src, &dst);
        }
    }

    // show sprites
    //

    // show player
    // debug player for now
    // player should be in like "engine->player" or some shit like that
    SDL_FRect dst = {
        (float)(window_width/2),
        (float)(window_height/2),
        16.0f * 4.0f * scale,
        16.0f * 4.0f * scale
    };
    SDL_FRect src = {
        16.0f*0, // frame in spritesheet, change to player->state
        0,
        16,
        16
    };
    SDL_RenderTexture(renderer, sprites_textures[1], &src, &dst);
    // sprites_textures[1] is the player sprite


    SDL_RenderPresent(renderer);
}

