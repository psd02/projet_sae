//
// Created by thoma on 29/09/2026.
//

#include "Engine.h"

#include <fstream>
#include <iostream>
#include <random>

#include "Affichage.h"
#include "json.hpp"

Engine::Engine() {
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
    for (auto value:inventory) {
        j["inventory"].push_back({
            {"object_type", value.object_type},
            {"amount", value.amount}
        });
    }

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
            for (auto value:info["inventory"]) {
                inventory.push_back({value["object_type"], value["amount"]});
            }
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

void Engine::generate() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> roll(1, 1000);

    // check each tile and do shit with it
    for (int y=0; y<MAP_HEIGHT; y++) {
        for (int x=0; x<MAP_WIDTH; x++) {
            switch (map[y][x]) {
                case GRASS:
                    // 0.1% chance of spawning a tree
                    if (roll(gen) == 1) {
                        std::cout << "Spawning a tree: " << x << " : "<<y << std::endl;
                        sprites.push_back({(float)x,(float)y,TREE,0});
                        sprites[0].x=x;
                        sprites[0].y=y;
                    }
                    break;
                case SAND:
                    // 0.1% chance of spawning a rock
                    if (roll(gen) == 1) {
                        uint8_t state = roll(gen)%3;
                        std::cout << "Spawning a rock: " << x << " : "<<y << ": " << state << std::endl;
                        sprites.push_back({(float)x,(float)y,ROCK,state});
                        //sprites[0].x=x;
                        //sprites[0].y=y;
                    }
                    break;
            }
        }
    }
}

void Engine::process() {
    // check if in menu
    if (isRenderingMenu) {
        if (start) {
            // close menu
            isRenderingMenu = false;
        }
        return;
    }

    // check if a key is pressed
    if (!isAnimating_walk) {
        if (up) {
            isAnimating_walk=true;
            sprites[0].state = 20;
            targetx=sprites[0].x;
            targety=sprites[0].y-1;
        } else if (down) {
            isAnimating_walk=true;
            sprites[0].state = 0;
            targetx=sprites[0].x;
            targety=sprites[0].y+1;
        } else if (left) {
            isAnimating_walk=true;
            sprites[0].state = 30;
            targetx=sprites[0].x-1;
            targety=sprites[0].y;
        } else if (right) {
            isAnimating_walk=true;
            sprites[0].state = 10;
            targetx=sprites[0].x+1;
            targety=sprites[0].y;
        }
    }

    if (sprites[0].x<0) {
        sprites[0].x = 0;
    } else if (sprites[0].x>=768){
        sprites[0].x=767;
    }
    if (sprites[0].y<0) {
        sprites[0].y = 0;
    } else if (sprites[0].y>=768) {
        sprites[0].y = 767;
    }

    // rest of the engine (probably nothing except animations lol)

    // for sprite states, first digit is row, second is collumn
    if (isAnimating_walk) {
        // check for clock, each 100ms move x thing
        if (clock()%10==0) {
            switch ((int)sprites[0].state / 10) { // getting only the first digit
                case 0: // down
                    sprites[0].y+=0.1;
                    break;
                case 1: // right
                    sprites[0].x+=0.1;
                    break;
                case 2: // up
                    sprites[0].y-=0.1;
                    break;
                case 3: // left
                    sprites[0].x-=0.1;
                    break;
            }
        }

        // show correct sprite
        if (sprites[0].state%10 ==0) {
            sprites[0].state+=1;
        }

        // return part
        if (abs(sprites[0].x-targetx)<=0.001 && abs(sprites[0].y-targety)<=0.001) {
            isAnimating_walk=false;
            if (sprites[0].state%10 == 1) {
                sprites[0].state-=1;
            }
            sprites[0].x=targetx;
            sprites[0].y=targety;
        }
    }
}
