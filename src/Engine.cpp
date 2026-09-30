//
// Created by thoma on 29/09/2026.
//

#include "Engine.h"

#include <fstream>
#include <iostream>

#include "Affichage.h"
#include "json.hpp"

Engine::Engine() {
    // set controls to false, just to be sure

    up, down, left, right = false;

    // load the sprites from the json
    // if nothing found then just create a player and set the default position on the map (prepare for generation)
    load_progress();
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

int Engine::save_progress() {
    // new json element to transform the data into a string
    nlohmann::json j;

    // player data
    j["player"] = {
        {"x", sprites[0].x},
        {"y", sprites[0].y},
        {"sprite_id", sprites[0].sprite_id},
        {"state", sprites[0].state}
    };

    // other sprites data
    j["sprites"] = {};
    for (int i=1; i<sprites.size(); i++) {
        j["sprites"].push_back({
            {"x", sprites[i].x},
            {"y", sprites[i].y},
            {"sprite_id", sprites[i].sprite_id},
            {"state", sprites[i].state}
        });
    }

    // inventory
    // TODO
    j["inventory"] = {};

    // save data inside save.json
    std::ofstream output("save.json");

    if (!output)
    {
        std::cerr << "Failed to open save.json, unable to save the game\n";
        return 1;
    }

    std::string JSON{j.dump()};
    output.write(JSON.c_str(), JSON.size());
    return 0;
}

int Engine::load_progress() {
    std::ifstream file("save.json");
    if (!file) {
        std::cerr << "Failed to open save file, using default values instead\n";
        sprites.push_back({0,0,0,0}); // it should be sprites[0], aka the player
    } else {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();

        nlohmann::json info = nlohmann::json::parse(content);

        //player
        if (info["player"]!=nullptr) {
            //load player position
            sprites.push_back({info["player"]["x"], info["player"]["y"], 0, info["player"]["state"]});
        } else {
            std::cerr << "No player data found inside the save file, using default values instead";
            sprites.push_back({0,0,0,0});
        }

        if (info["inventory"]!=nullptr) {
            // load inventory
            // TODO
        }

        if (info["sprites"]!=nullptr) {
            // load all sprites
            for (auto value: info["sprites"]) {
                sprites.push_back({value["x"], value["y"], value["sprite_id"],value["state"]});
            }
        }
    }
    file.close();
    return 0;
}

void Engine::process() {
    // check if a key is pressed
    if (up) {
        std::cout << "up\n";
    } else if (down) {
        std::cout << "down\n";
    } else if (left) {
        std::cout << "left\n";
    } else if (right) {
        std::cout << "right\n";
    }
}
