//
// Created by thoma on 14/09/2026.
//

#ifndef PROJET_SAE_AFFICHAGE_H
#define PROJET_SAE_AFFICHAGE_H

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

#include "background_sprite.h"
#include "background_sprite_path.h"

#define REFERENCE_WINDOW_HEIGHT 720
#define REFERENCE_WINDOW_WIDTH 1080
#define MAP_WIDTH 1024
#define MAP_HEIGHT 1024

// FIX: on définit combien de tuiles sont réellement chargées pour l'instant,
// séparément du nombre total défini dans les enums. Ça sert de "garde-fou"
// dans Affichage.cpp pour ne jamais utiliser un index de texture non chargée.
#define LOADED_TILE_COUNT 3
#define LOADED_SPRITE_COUNT 3

class Affichage {
public:
    Affichage();
    ~Affichage(); // FIX: maintenant réellement implémenté dans le .cpp (voir plus bas).
                  // Avant, il était déclaré mais jamais défini : ça ne plantait
                  // pas la compilation (tant que personne ne détruisait l'objet),
                  // mais ça veut dire qu'aucune ressource SDL n'était jamais libérée
                  // correctement -> fuite mémoire / ressources GPU à la fermeture.

    //methods
    void process();
    //vars
    bool running=true;

private:
    SDL_Window *window = nullptr;     // FIX: on ne crée plus la fenêtre "en ligne" ici.
    SDL_Renderer *renderer = nullptr; // On le fait dans le corps du constructeur
                                       // pour pouvoir vérifier si ça a échoué
                                       // (SDL_CreateWindow peut retourner nullptr,
                                       // et avant, rien ne le vérifiait : le code
                                       // continuait quand même et plantait plus loin
                                       // avec un message d'erreur très peu clair).

    void draw_background();
    void draw_sprites();
    // FIX: ces deux méthodes étaient déclarées mais jamais implémentées,
    // et jamais appelées non plus (tout le rendu est fait directement dans
    // process()). Je les laisse déclarées ici comme "TODO" pour vous en
    // discuter avec ton collègue : soit vous les implémentez et les
    // appelez depuis process(), soit vous les enlevez pour ne pas garder
    // du code mort. Pour l'instant le .cpp ne les définit pas exprès
    // (sinon on perd l'info qu'il reste du travail ici).

    //just declaring those so I can change them, instead of making new variable each frame
    int window_width=1080;
    int window_height=720;

    // FIX: la "texture de test" (img_test / test) a été supprimée.
    // Elle n'était utilisée nulle part dans process(), et en plus elle était
    // chargée comme initialiseur de membre AVANT que le constructeur ait pu
    // vérifier que renderer n'était pas nullptr -> risque de crash au tout
    // premier lancement si SDL_CreateRenderer échoue. Si vous voulez une
    // texture de debug, mieux vaut la charger dans le corps du constructeur,
    // après avoir vérifié que renderer est valide.

    // textures
    // FIX: la taille des tableaux correspond maintenant au nombre total de
    // tuiles/sprites définis dans les enums (background_sprite.h), plus
    // besoin de "5 // change later" en dur.
    SDL_Texture *tiles_textures[BACKGROUND_TILE_COUNT] = {nullptr};
    SDL_Texture *sprites_textures[SPRITE_TILE_COUNT] = {nullptr};

    //map
    uint8_t map[MAP_HEIGHT][MAP_WIDTH] {0};
};


#endif //PROJET_SAE_AFFICHAGE_H