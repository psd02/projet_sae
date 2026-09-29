//
// Created by thoma on 29/09/2026.
//

#include "Engine.h"

#include <fstream>
#include <iostream>

#include "Affichage.h"
#include "json.hpp"

Engine::Engine() {
    // load the sprites from the json
    // if nothing found then just create a player and set the default position on the map (prepare for generation)
    std::ifstream file("save.json");
    if (!file) {
        std::cerr << "Failed to open save file, using default values instead\n";
        sprites.push_back({0,0,0,0}); // it should be sprites[0], aka the player
    } else {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::cout << content << std::endl;

        //
    }
}

Engine::~Engine() {
    //
}

void Engine::add_sprite(Sprite sprite) {
    sprites.push_back(sprite);
}

void Engine::remove_sprite(Sprite sprite) {
    //
}

void Engine::process() {
    // check if a key is pressed
    if (up) {
        std::cout << "up\n";
    } else if (down) {
        std::cout << "down\n";
    } else if (left) {
        //
    } else if (right) {
        //
    }
}
