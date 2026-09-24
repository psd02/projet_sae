//
// Created by thoma on 14/09/2026.
//

#ifndef PROJET_SAE_BACKGROUND_SPRITE_H
#define PROJET_SAE_BACKGROUND_SPRITE_H

#include <cstdint> // FIX: uint8_t est utilisé plus bas (dans Sprite), il faut inclure
                    // ce header pour que ça compile même si personne d'autre ne l'a
                    // déjà inclus avant. Avant ça marchait "par chance" parce que
                    // SDL3.h était inclus ailleurs et l'incluait indirectement.

// FIX: on retire le ";" en trop après le dernier élément des enums.
// Ce n'est pas une erreur de compilation en C++ (contrairement au C strict),
// mais autant être propre.

typedef enum BackgroundTiles {
    MISSING = 0,
    GRASS = 1,
    DIRT = 2,
    SAND = 3,
    WATER = 4,
    RIVER_WATER = 5
} BackgroundTiles; // FIX: on nomme le typedef, ça permet d'écrire "BackgroundTiles tile;"
// au lieu de "enum BackgroundTiles tile;" partout.

typedef enum SpriteTiles {
    MISSING_SPRITE = 0,
    PLAYER = 1,
    BLANK = 2,
    TREE = 3,
    ROCK = 4,
    BUSH = 5
} SpriteTiles;

// FIX: on ajoute ces deux constantes pour ne plus jamais avoir à compter
// "à la main" combien il y a de tuiles / sprites déclarés. Si tu ajoutes
// une valeur dans l'enum, pense à incrémenter ce nombre (ou mieux, on
// pourrait faire un "BACKGROUND_COUNT" en dernier élément de l'enum,
// mais on reste simple pour l'instant).
#define BACKGROUND_TILE_COUNT 6
#define SPRITE_TILE_COUNT 6

struct Sprite {
    int x;
    int y;
    uint8_t sprite_id = 0;
    uint8_t state = 0; // sprite number in the sprite sheet,
    //if the sprite sheet is only 1 sprite this variable will get ignored, otherwise it will loop
};

#endif //PROJET_SAE_BACKGROUND_SPRITE_H