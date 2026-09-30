//
// Created by thoma on 29/09/2026.
//

#ifndef PROJET_SAE_ENGINE_H
#define PROJET_SAE_ENGINE_H

#include <vector>
#include "background_sprite.h"


class Engine {
public:
    Engine();
    ~Engine();
    int save_progress();
    int load_progress();
    void process();
    void add_sprite(Sprite sprite);
    void remove_sprite(Sprite sprite);
    // kinda janky, but works, so fuck it
    bool left, right, up, down;
private:
    // list of sprites used by the program
    // sprite 0 will ALWAYS be the player and sprite ID wont affect anything
    // only the state will affect the sprite
    std::vector<Sprite> sprites;
    //std::vector<Object> inventory;
    // TODO: struct Object
    bool isAnimating = false;
};


#endif //PROJET_SAE_ENGINE_H
