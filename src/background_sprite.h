//
// Created by thoma on 14/09/2026.
//

#ifndef PROJET_SAE_BACKGROUND_SPRITE_H
#define PROJET_SAE_BACKGROUND_SPRITE_H

#include <cstdint>

typedef enum BackgroundTiles {
    MISSING = 0,
    GRASS = 1,
    DIRT = 2,
    CLIFF = 3,
    WATER = 4,
    WATER_SHALLOW = 5,
    SAND = 6,
    DEV_WALL = 7
};

typedef enum SpriteTiles {
    MISSING_SPRITE = 0,
    PLAYER = 1,
    BLANK = 2,
    TREE = 3,
    ROCK = 4,
    BUSH = 5,
};

struct Sprite {
    int x;
    int y;
    uint8_t sprite_id = 0;
    uint8_t state = 0; // sprite number in the sprite sheet,
    //if the sprite sheet is only 1 sprite this variable will get ignored, otherwise it will loop
};

#endif //PROJET_SAE_BACKGROUND_SPRITE_H
