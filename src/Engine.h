//
// Created by thoma on 29/09/2026.
//

#ifndef PROJET_SAE_ENGINE_H
#define PROJET_SAE_ENGINE_H

#define MAP_WIDTH 768
#define MAP_HEIGHT 768

#include <vector>
#include "background_sprite.h"
#include "objects.h"

class Engine {
public:
    Engine();
    ~Engine();
    int save_progress();
    int load_progress();
    void process();
    void add_sprite(Sprite sprite);
    void remove_sprite(Sprite sprite);
    void generate(); // generate the sprites, trees, rock, etc...


    // kinda janky, but works, so fuck it
    bool left = false;
    bool right = false;
    bool up = false;
    bool down = false;
    bool start = false;

    // info for the GUI
    bool isRenderingMenu = false;
    bool isRenderingMainMenu = true;
    bool isRenderingInventory = false;

    // list of sprites used by the program
    // sprite 0 will ALWAYS be the player and sprite ID wont affect anything
    // only the state will affect the sprite
    std::vector<Sprite> sprites;
    std::vector<Object> inventory;

    bool isAnimating_walk = false;

    //map
    uint8_t map[MAP_HEIGHT][MAP_WIDTH] {0};

    // for temp variables
    float targetx;
    float targety;
};


#endif //PROJET_SAE_ENGINE_H
