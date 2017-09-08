#include "engine.hpp"

/**
* fonction principale
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void run(Engine * engine){
    switch(engine->currentAction){
        case NEWGAME :
            initGame(engine);
            initMenu(engine);
            engine->currentAction = GAME;
        break;

        case GAME :
            playGame(engine);
        break;

        case MENU :
            playMenu(engine);
        break;

        case NEWGAMEAGAIN :
            freeSurface(engine);
            engine->currentAction = NEWGAME;
            SDL_GetError();
        break;
    }
}
