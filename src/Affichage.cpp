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
    // FIX: la fenêtre et le renderer sont maintenant créés ICI, avec une
    // vérification derrière. Avant, ils étaient créés comme valeurs par
    // défaut des membres dans le .h, sans aucun contrôle d'erreur : si
    // SDL_CreateWindow échouait (par exemple SDL_Init pas appelé avant !),
    // tout le reste du programme continuait avec un pointeur nullptr et
    // plantait n'importe où, sans message clair.
    //
    // ATTENTION: il manque SDL_Init(SDL_INIT_VIDEO) quelque part avant de
    // créer la fenêtre (normalement au tout début de main(), ou ici).
    // Je ne l'ajoute pas ici pour ne pas trop m'éloigner de la structure
    // d'origine, mais c'est probablement la toute première chose à corriger
    // avec ton collègue si SDL3_image nécessite aussi SDL_INIT_VIDEO actif.
    window = SDL_CreateWindow("Jeu", 1080, 720, SDL_WINDOW_RESIZABLE);
    if (window == nullptr) {
        std::cerr << "Failed to create window: " << SDL_GetError() << '\n';
        running = false;
        return; // FIX: on arrête tout de suite, pas la peine de continuer
                // à charger des textures sur un renderer qui n'existera pas.
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == nullptr) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << '\n';
        running = false;
        return;
    }

    // load map.bin
    std::ifstream file("assets/map.bin", std::ios::binary);

    if (!file) {
        std::cerr << "Failed to open file assets/map.bin\n";
    } else {
        uint8_t value;
        size_t index = 0; // FIX (bug principal du fichier) : avant, x et y étaient
                           // calculés à partir de la VALEUR de l'octet lu
                           // (value / MAP_HEIGHT, value % MAP_WIDTH). Comme value
                           // est un uint8_t (0-255) et MAP_HEIGHT = 1024, "y" valait
                           // TOUJOURS 0, et "x" valait toujours "value" lui-même.
                           // Résultat : seule la première ligne de la carte (et
                           // seulement les 256 premières colonnes) était remplie,
                           // avec des valeurs redondantes. Il fallait utiliser la
                           // POSITION dans le fichier (donc un compteur), pas la
                           // valeur de l'octet.
        while (file.get(reinterpret_cast<char&>(value))) {
            uint16_t y = index / MAP_WIDTH;
            uint16_t x = index % MAP_WIDTH;
            if (y < MAP_HEIGHT && x < MAP_WIDTH) { // FIX: sécurité si le fichier
                                                    // est plus grand que la carte.
                map[y][x] = value;
            }
            index++;
        }
    }

    // load textures
    for (int i = 0; i < LOADED_TILE_COUNT; i++) { // FIX: on utilise la constante
                                                    // au lieu du "3" en dur, pour
                                                    // que ce soit cohérent avec
                                                    // Affichage.h si jamais on
                                                    // charge plus de tuiles plus tard.
        SDL_Surface* surface = IMG_Load(background_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << background_path[i] << ", defaulting to missing.png\n";
            surface = IMG_Load(background_path[MISSING]);
        }
        if (surface != nullptr) { // FIX: avant, si même missing.png échouait à
                                   // charger, on appelait quand même
                                   // SDL_CreateTextureFromSurface(renderer, nullptr),
                                   // ce qui est un comportement indéfini côté SDL.
            tiles_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface); // FIX (fuite mémoire) : la surface chargée
                                          // par IMG_Load n'était jamais libérée. Une
                                          // fois la texture GPU créée, on n'a plus
                                          // besoin de la surface RAM.
        } else {
            std::cerr << "Failed to load even the fallback missing.png texture!\n";
        }
    }

    for (int i = 0; i < LOADED_SPRITE_COUNT; i++) { // FIX: idem, constante au lieu de "3".
        SDL_Surface* surface = IMG_Load(sprite_path[i]);
        if (surface == nullptr) {
            std::cerr << "Failed to load texture: " << sprite_path[i] << ", defaulting to missing.png\n";
            surface = IMG_Load(sprite_path[MISSING_SPRITE]);
        }
        if (surface != nullptr) {
            sprites_textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_DestroySurface(surface); // FIX: même fuite corrigée ici.
        } else {
            std::cerr << "Failed to load even the fallback missing.png sprite!\n";
        }
    }
}

// FIX: destructeur enfin implémenté. Il détruit dans l'ordre inverse de
// création (textures, puis renderer, puis fenêtre) : c'est la convention
// SDL classique, pour éviter de détruire un objet dont un autre dépend
// encore.
Affichage::~Affichage() {
    for (int i = 0; i < BACKGROUND_TILE_COUNT; i++) {
        if (tiles_textures[i] != nullptr) {
            SDL_DestroyTexture(tiles_textures[i]);
        }
    }
    for (int i = 0; i < SPRITE_TILE_COUNT; i++) {
        if (sprites_textures[i] != nullptr) {
            SDL_DestroyTexture(sprites_textures[i]);
        }
    }
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (window != nullptr) {
        SDL_DestroyWindow(window);
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

    // FIX (bug important) : les bornes de boucle étaient "y < 0+5" et
    // "x < 0+5", c'est-à-dire "< 5" tout court, sans aucun lien avec
    // playerx/playery. Du coup le rendu ne suivait jamais le joueur : on
    // dessinait toujours les mêmes tuiles proches de (0,0), peu importe où
    // playerx/playery se trouvaient. Ici on calcule un rayon de tuiles
    // visibles autour du joueur, basé sur la taille de la fenêtre, pour que
    // la zone dessinée suive réellement le joueur et remplisse l'écran.
    int tile_size = (int)(16 * 4 * scale);
    if (tile_size < 1) tile_size = 1; // FIX: sécurité anti-division par zéro si scale est très petit.
    int tiles_horizontal = window_width / tile_size + 2;  // +2 pour ne pas laisser de bord vide
    int tiles_vertical   = window_height / tile_size + 2;

    for (int y = playery - tiles_vertical / 2; y < playery + tiles_vertical / 2; y++) {
        for (int x = playerx - tiles_horizontal / 2; x < playerx + tiles_horizontal / 2; x++) {
            if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) {
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

            // FIX (bug potentiel/crash) : map[y][x] peut contenir n'importe
            // quelle valeur de 0 à 255 (lue depuis map.bin), mais seules
            // LOADED_TILE_COUNT textures sont réellement chargées. Avant,
            // SDL_RenderTexture(renderer, tiles_textures[map[y][x]], ...)
            // pouvait donc lire hors du tableau tiles_textures, ou utiliser
            // une texture jamais initialisée (nullptr) -> crash. On
            // sécurise avec un index de secours vers MISSING.
            uint8_t tile_id = map[y][x];
            if (tile_id >= LOADED_TILE_COUNT || tiles_textures[tile_id] == nullptr) {
                tile_id = MISSING;
            }

            SDL_RenderTexture(renderer, tiles_textures[tile_id], &src, &dst);
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
    SDL_RenderTexture(renderer, sprites_textures[PLAYER], &src, &dst); // FIX: on utilise
                                                                         // l'enum PLAYER au
                                                                         // lieu du "1" magique,
                                                                         // plus lisible et plus
                                                                         // sûr si l'ordre change.


    SDL_RenderPresent(renderer);
}