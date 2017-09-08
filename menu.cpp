#include "menu.hpp"
#include "game.hpp"
#include "player.hpp"

/**
* affichage du menu principal
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenu(Engine * engine){
    SDL_BlitSurface(engine->menu.background, NULL, engine->window, &engine->menu.positionBackground);

    for(int i = 0; i < NOMBRE_ITEM_MENU; i++){
        SDL_BlitSurface(engine->menu.menu[i].background, NULL, engine->window, &engine->menu.menu[i].positionBackground);
    }
}

/**
* fonction gérant les clics dans le menu principal
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void playMenu(Engine * engine){
    drawGame(engine);
    drawPieces(engine);
    drawPiecesDead(engine);
    drawMenu(engine);

    SDL_Event event;
    PositionItem position;
    int clickId;

    SDL_PollEvent(&event);
    switch(event.type){
        case SDL_QUIT:
            engine->currentAction = QUIT;
        break;

        case SDL_MOUSEBUTTONUP:
            position.x = event.button.x;
            position.y = event.button.y;

            clickId = checkClickMenu(engine->menu.menu, NOMBRE_ITEM_MENU, position);

            switch(clickId){
                case MENU_RETOUR_AU_JEU :
                    engine->currentAction = GAME;
                break;

                case MENU_NOUVELLE_PARTIE :
                    engine->currentAction = NEWGAMEAGAIN;
                break;

                case MENU_ENREGISTRER :
                    enregistrer(engine);
                break;

                case MENU_CHARGER :
                    charger(engine);
                break;

                case MENU_ABANDONNER :
                    menuMatchNul(engine);
                break;

                case MENU_QUITTER :
                    engine->currentAction = QUIT;
                break;
            }
        break;

        case SDL_KEYDOWN:
            if(event.key.keysym.sym == SDLK_ESCAPE){
                engine->currentAction = GAME;
            }
        break;
    }
}

/**
* fonction gérant l'enregistrement
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void enregistrer(Engine * engine){

}

/**
* fonction gérant le chargemnt d'une partie
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void charger(Engine * engine){

}
