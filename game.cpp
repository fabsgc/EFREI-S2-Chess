#include "game.hpp"
#include "player.hpp"

/**
* affiche les objets d'une partie
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawGame(Engine * engine){
    SDL_BlitSurface(engine->game.background, NULL, engine->window, &engine->game.positionBackground);
    writeMessage(engine);
}

/**
* affiche les objets d'une partie
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawPieces(Engine * engine){
    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            SDL_BlitSurface(engine->game.echiquier[i][j].background, NULL, engine->window, &engine->game.echiquier[i][j].positionBackground);
        }
    }
}

/**
* affiche les objets d'une partie
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawPiecesDead(Engine * engine){
    int nombrePieceBLanche = 0;
    int nombrePieceNoir = 0;

    for(int i = 0; i < engine->game.nombreTaken; i++){
        if(engine->game.taken[i].equipe == WHITE){
            if(nombrePieceBLanche < WIDTH_ARRAY){
                engine->game.taken[i].positionBackground.x = WIDTH_LEFT_PERDU;
                engine->game.taken[i].positionBackground.y = WIDTH_CASE*nombrePieceBLanche + WIDTH_TOP;
            }
            else{
                engine->game.taken[i].positionBackground.x = WIDTH_LEFT_PERDU + NOMBRE_CASE;
                engine->game.taken[i].positionBackground.y = WIDTH_CASE*(nombrePieceBLanche - WIDTH_ARRAY) + WIDTH_TOP;
            }

            nombrePieceBLanche++;
        }
        else if(engine->game.taken[i].equipe == BLACK){
            if(nombrePieceNoir < WIDTH_ARRAY){
                engine->game.taken[i].positionBackground.x = WIDTH_LEFT_PERDU + NOMBRE_CASE*2;
                engine->game.taken[i].positionBackground.y = WIDTH_CASE*nombrePieceNoir + WIDTH_TOP;
            }
            else{
                engine->game.taken[i].positionBackground.x = WIDTH_LEFT_PERDU + NOMBRE_CASE*3;
                engine->game.taken[i].positionBackground.y = WIDTH_CASE*(nombrePieceNoir - WIDTH_ARRAY) + WIDTH_TOP;
            }

            nombrePieceNoir++;
        }

        SDL_BlitSurface(engine->game.taken[i].background, NULL, engine->window, &engine->game.taken[i].positionBackground);
    }
}

/**
* affiche le menu des promotions
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenuPromotion(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    if(engine->game.tourPlayer == WHITE)
        SDL_BlitSurface(engine->game.menuPromotion.backgroundWhite, NULL, engine->window, &engine->game.menuPromotion.positionBackground);
    else
        SDL_BlitSurface(engine->game.menuPromotion.backgroundBlack, NULL, engine->window, &engine->game.menuPromotion.positionBackground);

    SDL_Flip(engine->window);
}

/**
* affiche le menu des echec et mat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenuEchecEtMat(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    if(engine->game.tourPlayer == WHITE)
        SDL_BlitSurface(engine->game.menuEchecEtMat.backgroundBlack, NULL, engine->window, &engine->game.menuEchecEtMat.positionBackground);
    else
        SDL_BlitSurface(engine->game.menuEchecEtMat.backgroundWhite, NULL, engine->window, &engine->game.menuEchecEtMat.positionBackground);

    SDL_Flip(engine->window);
}

/**
* affiche le menu pat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenuPat(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    SDL_BlitSurface(engine->game.menuPat.background, NULL, engine->window, &engine->game.menuPat.positionBackground);

    SDL_Flip(engine->window);
}

/**
* affiche le menu match nul
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenuMatchNul(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    SDL_BlitSurface(engine->game.menuMatchNul.background, NULL, engine->window, &engine->game.menuMatchNul.positionBackground);

    SDL_Flip(engine->window);
}

/**
* affiche le menu match nul
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenu50Coup(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    SDL_BlitSurface(engine->game.menu50Coup.background, NULL, engine->window, &engine->game.menu50Coup.positionBackground);

    SDL_Flip(engine->window);
}

/**
* affiche le menu match nul
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawMenu3Coup(Engine * engine){
    drawGame(engine);

    drawPieces(engine);
    drawPiecesDead(engine);

    SDL_BlitSurface(engine->game.menu3Coup.background, NULL, engine->window, &engine->game.menu3Coup.positionBackground);

    SDL_Flip(engine->window);
}

/**
* fonction principal gérant les tours de jeu
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void playGame(Engine * engine){
    if(engine->game.tourPlayer == 0){
        engine->game.message = "Aux blancs de jouer";
    }
    else{
        engine->game.message = "Aux noirs de jouer";
    }

    drawGame(engine);
    drawPieces(engine);
    drawPiecesDead(engine);
    playTour(engine);
}

/**
* affiche un carré vert en dessous d'une case où l'on peut se déplacer
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void drawPieceJouable(Engine * engine, int clickX, int clickY){
    drawGame(engine);

    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            if(engine->game.echiquierDeplacement[i][j] == 1){
                if(engine->game.echiquier[i][j].equipe == getEquipeAdverse(engine)){
                    SDL_BlitSurface(engine->game.surfaceCaseJouableEnnemi, NULL, engine->window, &engine->game.echiquier[i][j].positionBackground);
                }
                else{
                    SDL_BlitSurface(engine->game.surfaceCaseJouable, NULL, engine->window, &engine->game.echiquier[i][j].positionBackground);
                }
            }
        }
    }

    SDL_BlitSurface(engine->game.surfaceCaseSelected, NULL, engine->window, &engine->game.echiquier[clickX][clickY].positionBackground);

    drawPieces(engine);
    drawPiecesDead(engine);
    SDL_Flip(engine->window);
}
