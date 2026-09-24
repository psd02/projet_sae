//
// Created by thoma on 16/09/2026.
//

#ifndef PROJET_SAE_BACKGROUND_SPRITE_TILES_H
#define PROJET_SAE_BACKGROUND_SPRITE_TILES_H

#include "background_sprite.h" // FIX: pour utiliser BACKGROUND_TILE_COUNT / SPRITE_TILE_COUNT

// FIX (bug important) : l'enum BackgroundTiles va jusqu'à RIVER_WATER = 5,
// mais ce tableau n'avait que 3 entrées (index 0 à 2). Donc
// background_path[SAND], background_path[WATER] et background_path[RIVER_WATER]
// lisaient en dehors du tableau -> comportement indéfini (crash probable,
// ou pire, un crash aléatoire plus tard qui n'a rien à voir).
// On complète avec des placeholders qui pointent vers missing.png en
// attendant les vraies images. Remplace les chemins quand les assets existent.
inline const char* background_path[BACKGROUND_TILE_COUNT] = {
    "assets/missing.png",       // MISSING
    "assets/tiles/grass.png",   // GRASS
    "assets/tiles/dirt.png",    // DIRT
    "assets/missing.png",       // SAND       -- TODO: remplacer par assets/tiles/sand.png
    "assets/missing.png",       // WATER      -- TODO: remplacer par assets/tiles/water.png
    "assets/missing.png"        // RIVER_WATER -- TODO: remplacer par assets/tiles/river_water.png
};

// FIX: même problème pour les sprites (TREE, ROCK, BUSH manquaient).
inline const char* sprite_path[SPRITE_TILE_COUNT] = {
    "assets/missing.png",         // MISSING_SPRITE
    "assets/sprites/player.png",  // PLAYER
    "assets/sprites/blank.png",   // BLANK
    "assets/missing.png",         // TREE -- TODO
    "assets/missing.png",         // ROCK -- TODO
    "assets/missing.png"          // BUSH -- TODO
};

#endif //PROJET_SAE_BACKGROUND_SPRITE_TILES_H