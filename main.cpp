#include <iostream>

#include "src/Affichage.h"

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

int main() {
    // FIX (important) : rien n'initialisait SDL avant de créer une fenêtre.
    // SDL_CreateWindow / SDL_CreateRenderer peuvent échouer silencieusement
    // (ou planter selon les plateformes) si SDL_Init n'a pas été appelé
    // avant. C'est probablement la cause n°1 d'un "ça ne marche pas du
    // tout" en testant sur une autre machine.
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return 1;
    }

    Affichage *gui = new Affichage();

    // FIX: on utilise gui->running (déjà présent dans la classe Affichage,
    // mis à jour maintenant si la fenêtre/le renderer échouent à se créer)
    // au lieu d'une variable locale "running" complètement déconnectée de
    // l'état réel de l'objet. Avant, même si Affichage échouait à
    // s'initialiser, la boucle principale tournait quand même.
    while (gui->running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                gui->running = false;
            }
        }

        // game loop
        gui->process(); // update the gui

        // FIX: limite très simple de la boucle pour ne pas tourner à fond
        // les CPU/GPU sans aucune pause (ça ferait chauffer la machine et
        // rendrait le jeu injouable niveau vitesse, car pour l'instant la
        // logique tourne aussi vite que le CPU le permet, sans notion de
        // temps). C'est volontairement basique (pas de vrai delta-time) :
        // ça sert de garde-fou en attendant une vraie boucle de jeu avec
        // gestion du temps (SDL_GetTicks / delta time) que vous ajouterez
        // ensemble plus tard.
        SDL_Delay(16); // ~60 images par seconde
    }

    delete gui; // FIX (fuite mémoire) : gui était alloué avec "new" mais
                // jamais libéré. Le destructeur qu'on vient d'implémenter
                // dans Affichage ne sert à rien si on ne l'appelle jamais.

    SDL_Quit(); // FIX: on ferme proprement SDL à la fin, symétrique du SDL_Init.

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}