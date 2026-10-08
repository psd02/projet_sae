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

Affichage::Affichage(Engine& e) : engine(e) {
    // load map.bin
    std::ifstream file("assets/map.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file assets/map.bin\n";
    } else {
        uint8_t value;
        size_t index = 0;

        while (file.read(reinterpret_cast<char*>(&value), sizeof(value))) {

            if (index >= MAP_WIDTH * MAP_HEIGHT) {
                std::cerr << "Map file is too large!\n";
                break;
            }

            uint16_t x = index % MAP_WIDTH;
            uint16_t y = index / MAP_WIDTH;

            map[y][x] = value;

            index++;
        }
    }

    // load textures
    for (int i = 0; i <= 7; i++) {
        // only 3 tiles for now
        // load texture
        SDL_Surface *surface = IMG_Load(background_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << background_path[i] << ", defaulting to missing.png\n";
            //missing.png should be saved inside the code tbh
            surface = IMG_Load(background_path[0]);
            tiles_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        } else {
            tiles_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        }
        if (tiles_textures[i]) {
            SDL_SetTextureScaleMode(tiles_textures[i], SDL_SCALEMODE_NEAREST);
        }
    }

    for (int i = 0; i < 3; i++) {
        // only 3 sprites for now
        // load texture
        SDL_Surface *surface = IMG_Load(sprite_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << sprite_path[i] << ", defaulting to missing.png\n";
            //missing.png should be saved inside the code tbh
            surface = IMG_Load(sprite_path[0]);
            sprites_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        } else {
            sprites_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        }
        if (sprites_textures[i]) {
            SDL_SetTextureScaleMode(sprites_textures[i], SDL_SCALEMODE_NEAREST);
        }
    }
}

void Affichage::process() {
    // after x time passed
    background_animation_state!=background_animation_state;

    SDL_RenderClear(renderer);

    //get window size
    SDL_GetWindowSize(window, &window_width, &window_height);

    float scale_x = (float)window_width  / REFERENCE_WINDOW_WIDTH;
    float scale_y = (float)window_height / REFERENCE_WINDOW_HEIGHT;
    float scale = fminf(scale_x, scale_y); // chose the smallest number between 2 floats

    //debug while waiting for real player position
    float_t playerx = engine.sprites[0].x;
    float_t playery = engine.sprites[0].y;

    // show background tiles
    float_t cameraX = playerx * (16*4*scale) - window_width / 2;
    float_t cameraY = playery * (16*4*scale) - window_height / 2;
    for (int y=playery-16; y<playery+16; y++) { // 0 is player Y
        for (int x=playerx-16; x<playerx+16; x++) { // 0 is player X
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

            SDL_FRect src;
            // for water tile
            if (map[y][x] == WATER || map[y][x] == WATER_SHALLOW) {
                int multiplier = 0;
                if ((clock()%1500)>=1000) {
                    multiplier = 2;
                } else if ((clock()%1500)>=500) {
                    multiplier = 1;
                } else {
                    multiplier = 0;
                }
                src = {
                    16.0f*multiplier, // frame in spritesheet
                    0,
                    16,
                    16
                };
            } else {
                src = {
                    16.0f*0, // frame in spritesheet
                    0,
                    16,
                    16
                };
            }

            SDL_RenderTexture(renderer, tiles_textures[map[y][x]], &src, &dst);
        }
    }

    // show sprites
    for (int i=1; i<engine.sprites.size(); i++) {
        // show everything but the player


        SDL_FRect dst = {
            (float)engine.sprites[i].x*16*4*scale - (float)cameraX,
            (float)engine.sprites[i].y*16*4*scale - (float)cameraY,
            16.0f * 4.0f * scale,
            16.0f * 4.0f * scale
        };
        SDL_FRect src = {
            16.0f*0, // frame in spritesheet, change to player->state
            0,
            16,
            16
        };
        SDL_RenderTexture(renderer, sprites_textures[engine.sprites[i].sprite_id], &src, &dst);
    }

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
        16.0f*engine.sprites[0].state, // frame in spritesheet, change to player->state
        0,
        16,
        16
    };
    SDL_RenderTexture(renderer, sprites_textures[1], &src, &dst);
    // sprites_textures[1] is the player sprite


    SDL_RenderPresent(renderer);
}

