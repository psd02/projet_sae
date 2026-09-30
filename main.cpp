#include <iostream>

#include "src/Affichage.h"
#include "src/Engine.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    Engine *engine = new Engine();
    Affichage *gui = new Affichage();

    bool running = true;
    while (running) {
        // this thing bellow will be used for controls AND closing the game
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.key) {
                    default:
                        break;
                    case 122: // 122 is Z
                        engine->up = true;
                        break;
                    case 1073741906: // up arrow
                        engine->up = true;
                        break;
                    case 115: // S
                        engine->down = true;
                        break;
                    case 1073741905: // down arrow
                        engine->down = true;
                        break;

                    case 113: // 113 is Q
                        engine->left = true;
                        break;
                    case 1073741904: // left arrow
                        engine->left = true;
                        break;
                    case 100: // D
                        engine->right = true;
                        break;
                    case 1073741903: // right arrow
                        engine->right = true;
                        break;
                }
                // this is for debugging and finding out which key is what
                std::cout << event.key.key << std::endl;
            }

            if (event.type == SDL_EVENT_KEY_UP) {
                switch (event.key.key) {
                    default:
                        break;
                    case 122: // 122 is Z
                        engine->up = false;
                        break;
                    case 1073741906: // up arrow
                        engine->up = false;
                        break;
                    case 115: // S
                        engine->down = false;
                        break;
                    case 1073741905: // down arrow
                        engine->down = false;
                        break;

                    case 113: // 113 is Q
                        engine->left = false;
                        break;
                    case 1073741904: // left arrow
                        engine->left = false;
                        break;
                    case 100: // D
                        engine->right = false;
                        break;
                    case 1073741903: // right arrow
                        engine->right = false;
                        break;
                }
            }
        }


        // game loop
        gui->process(); // update the gui
        engine->process();
    }
    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}