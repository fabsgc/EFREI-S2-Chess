#include "player.hpp"

SDL_Surface * emptyCase = IMG_Load(SPRITE_EMPTY);

/**
* boucle du joueur
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void playTour(Engine * engine){
    SDL_Event event;
    SDL_Event eventPlay;
    PositionItem position;
    int clickId, clicX, clicY, clicXFinal, clicYFinal;
    bool finished = false; //d�finit si le tour est termin� ou non
    bool finishedSelectionCase = false; //d�finit si la s�lection de la case de destination est termin�e

    //##########################################################################################################
    while(finished == false){
        finishedSelectionCase = false;
        clearTabDeplacement(engine);

        engine->game.enEchec = false;
        engine->game.enEchecEtMat = false;
        engine->game.enPat = false;

        if(checkEchecEtMat(engine) == true){
            finished = true;
            menuEchecEtMat(engine);
        }
        else if(checkPat(engine) == true){
            finished = true;
            menuPat(engine);
        }
        else{
            SDL_PollEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    finished = true;
                    engine->currentAction = QUIT;
                break;

                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_LEFT){
                        position.x = event.button.x;
                        position.y = event.button.y;

                        clickId = checkClickMenu(engine->game.menu, NOMBRE_ITEM_MENU_GAME, position);

                        //click sur le menu
                        if(clickId != ERROR){
                            finished  = true;
                            engine->currentAction = MENU;
                        }
                        else{
                            //boucle principale
                            //on recupere la case en X et en Y qui a �t� cliqu�e par le joueur
                            clicX = getCoordClickX(engine, position);
                            clicY = getCoordClickY(engine, position);

                            if(clicY != ERROR && clicX != ERROR){

                                //verifier si on a clique sur une de ses pi�ces
                                if(checkIfIsYourPiece(engine, clicX, clicY) == true){

                                    //calcul des d�placements possibles
                                    calculDeplacementPiece(engine, clicX, clicY);
                                    checkPriseEnPassant(engine, clicX, clicY);
                                    checkRoque(engine, clicX, clicY);

                                    //on teste les 3 donn�es
                                    checkEchec(engine, clicX, clicY);

                                    drawPieceJouable(engine, clicX, clicY);
                                    //##########################################################################################################
                                    //on boucle sur la s�lection de la future case
                                    while(finishedSelectionCase == false){
                                        SDL_WaitEvent(&eventPlay);
                                        switch(eventPlay.type){
                                            case SDL_QUIT:
                                                finished = true;
                                                finishedSelectionCase = true;
                                                engine->currentAction = QUIT;
                                            break;

                                            case SDL_MOUSEBUTTONDOWN:
                                                if(eventPlay.button.button == SDL_BUTTON_LEFT){
                                                    position.x = eventPlay.button.x;
                                                    position.y = eventPlay.button.y;

                                                    clicXFinal = getCoordClickX(engine, position);
                                                    clicYFinal = getCoordClickY(engine, position);

                                                    //verifier si le d�placement est correct
                                                    if(checkMoveAvailable(engine, clicXFinal, clicYFinal) == true){
                                                        deplacePiece(engine, clicX, clicY, clicXFinal, clicYFinal, true);
                                                        checkPromotion(engine, clicXFinal, clicYFinal);

                                                        finished = true;
                                                        finishedSelectionCase = true;
                                                        engine->game.tourPlayer = getEquipeAdverse(engine);
                                                        engine->game.roiRoquePossible = false;

                                                        //r�gle des 50 coups
                                                        if(engine->game.nombreCoupLastCapturePromotion + 50 <= engine->game.nombreDeplacement){
                                                            menu50Coup(engine);
                                                        }
                                                    }
                                                    else{
                                                        finished  = true;
                                                        finishedSelectionCase = true;
                                                    }
                                                }
                                            break;

                                            case SDL_KEYDOWN:
                                                if(eventPlay.key.keysym.sym == SDLK_ESCAPE){
                                                    finished  = true;
                                                    finishedSelectionCase = true;
                                                }
                                            break;
                                        }
                                    }
                                    //##########################################################################################################
                                }
                            }

                            SDL_Flip(engine->window);
                        }
                    }
                break;

                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_ESCAPE){
                        finished  = true;
                        engine->currentAction = MENU;
                    }
                break;

                default:
                    finished  = true;
                break;
            }
        }
    }
    //##########################################################################################################
}

/**
* retourne la coordonn�e X de la case cliqu�e
* @param Engine * engine : pointeur vers la structure principale
* @param PositionItem positionClick : position du click
* @access public
* @return int
*/
int getCoordClickX(Engine * engine, PositionItem positionClick){
    for(int i = 0; i < WIDTH_ARRAY; i++){
        if(positionClick.x >= engine->game.echiquier[i][1].position.x && positionClick.x <= engine->game.echiquier[i][1].position.x + WIDTH_CASE){
            return i;
        }
    }

    return ERROR;
}

/**
* retourne la coordonn�e Y de la case cliqu�e
* @param Engine * engine : pointeur vers la structure principale
* @param PositionItem positionClick : position du click
* @access public
* @return int
*/
int getCoordClickY(Engine * engine, PositionItem positionClick){
    for(int i = 0; i < WIDTH_ARRAY; i++){
        if(positionClick.y >= engine->game.echiquier[1][i].position.y && positionClick.y <= engine->game.echiquier[1][i].position.y + WIDTH_CASE){
            return i;
        }
    }

    return ERROR;
}

/**
* v�rifie si la pi�ce appartien au joueur
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @access public
* @return bool
*/
bool checkIfIsYourPiece(Engine * engine, int clickX, int clickY){
    if(engine->game.echiquier[clickX][clickY].equipe == engine->game.tourPlayer){
        return true;
    }
    else{
        return false;
    }
}

/**
* v�rifie si le d�placement est correct
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @access public
* @return bool
*/
bool checkMoveAvailable(Engine * engine, int clickX, int clickY){
    if(clickX >= 0 && clickX < WIDTH_ARRAY && clickY >= 0 && clickY < WIDTH_ARRAY){
        if(engine->game.echiquierDeplacement[clickX][clickY] == 1){
            return true;
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

/**
* Calcul les d�placements de la pi�ce entr�es et les enregistre dans un tableau 8*8 enregistr� dans la structure game
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @access public
* @return bool
*/
void calculDeplacementPiece(Engine * engine, int clickX, int clickY){
    engine->game.nombreDeplacementPossible = 0;

    switch(engine->game.echiquier[clickX][clickY].type){
        case PION :
            deplacePion(engine, clickX, clickY, &engine->game);
        break;

        case CAVALIER :
            deplaceCavalier(engine, clickX, clickY, &engine->game);
        break;

        case FOU :
            deplaceFou(engine, clickX, clickY, &engine->game);
        break;

        case TOUR :
            deplaceTour(engine, clickX, clickY, &engine->game);
        break;

        case DAME :
            deplaceDame(engine, clickX, clickY, &engine->game);
        break;

        case ROI :
            deplaceRoi(engine, clickX, clickY, &engine->game);
        break;
    }

    //on vire la capture du roi
    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            if(engine->game.echiquier[i][j].type == ROI && engine->game.echiquierDeplacement[i][j] == D_POSSIBLE){
                engine->game.echiquierDeplacement[i][j] = D_MISE_EN_ECHEC;
            }
        }
    }
}

/**
* retourne l'�quipe actuelle
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return int
*/
int getMonEquipe(Engine * engine){
    if(engine->game.tourPlayer == WHITE){
        return WHITE;
    }
    else{
        return BLACK;
    }
}

/**
* retourne l'�quipe adverse
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return int
*/
int getEquipeAdverse(Engine * engine){
    if(engine->game.tourPlayer == WHITE){
        return BLACK;
    }
    else{
        return WHITE;
    }

    return ERROR;
}

/**
* d�place le pion a l'endroit pr�vu et met � jour les donn�es des 2 cases
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param int clickXFinal : coordonn�e X de la case d'arriv�e
* @param int clickYFinal : coordonn�e Y de la pi�ce d'arriv�e
* @param bool changeBackground : est ce que l'on doit changer le background des pi�ces (utile pour les calculs invisibles)
* @access public
* @return void
*/
void deplacePiece(Engine * engine, int clickX, int clickY, int clickXFinal, int clickYFinal, bool changeBackground){
    string sprite = SPRITE_EMPTY;
    char * cstr = new char[sprite.length() + 1];
    strcpy(cstr, sprite.c_str());

    //la case contenait-elle une pi�ce adverse ?
    if(engine->game.echiquier[clickXFinal][clickYFinal].equipe == getEquipeAdverse(engine)){
        engine->game.taken[engine->game.nombreTaken].type = engine->game.echiquier[clickXFinal][clickYFinal].type;
        engine->game.taken[engine->game.nombreTaken].equipe = getEquipeAdverse(engine);
        if(changeBackground == true) engine->game.taken[engine->game.nombreTaken].background = engine->game.echiquier[clickXFinal][clickYFinal].background;
        engine->game.nombreTaken++;
        engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une capture
    }

    //pour la prise en passant (valeur de case 2), on vide la case et on ajoute la pi�ce perdue si la prise en passant a �t� jou�e
    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            if(engine->game.echiquierDeplacement[i][j] == D_PRISE_EN_PASSANT && clickX != clickXFinal){
                engine->game.taken[engine->game.nombreTaken].type = engine->game.echiquier[j][j].type;
                engine->game.taken[engine->game.nombreTaken].equipe = getEquipeAdverse(engine);
                if(changeBackground == true) engine->game.taken[engine->game.nombreTaken].background = engine->game.echiquier[i][j].background;
                engine->game.nombreTaken++;
                engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une prise en passant

                engine->game.echiquier[i][j].type = EMPTY;
                engine->game.echiquier[i][j].equipe = EMPTY_TEAM;
                if(changeBackground == true) engine->game.echiquier[i][j].background = emptyCase;
            }
        }
    }

    //pour le roque on va checker pour les 2 positions X du roi possibles
    //si la tour est � c�t� et que le roque est possible, on la d�place
    if(engine->game.echiquier[clickX][clickY].type == ROI){
        if(clickX+3 < WIDTH_ARRAY
        && engine->game.echiquier[clickX+3][clickY].type == TOUR
        && engine->game.echiquierDeplacement[clickX+3][clickY] == D_ROQUE
        && clickXFinal == clickX+2){
            engine->game.echiquier[clickX+1][clickY].type = engine->game.echiquier[clickX+3][clickY].type;
            engine->game.echiquier[clickX+1][clickY].equipe = engine->game.echiquier[clickX+3][clickY].equipe;
            if(changeBackground == true) engine->game.echiquier[clickX+1][clickY].background = engine->game.echiquier[clickX+3][clickY].background;
            engine->game.echiquier[clickX+1][clickY].nombreDeplacement = 1;
            engine->game.echiquier[clickX+3][clickY].type = EMPTY;
            engine->game.echiquier[clickX+3][clickY].equipe = EMPTY_TEAM;
            if(changeBackground == true) engine->game.echiquier[clickX+3][clickY].background = emptyCase;
        }
        else if(clickX-4 >= 0
        && engine->game.echiquier[clickX-4][clickY].type == TOUR
        && engine->game.echiquierDeplacement[clickX-4][clickY] == D_ROQUE
        && clickXFinal == clickX-3){
            engine->game.echiquier[clickX-2][clickY].type = engine->game.echiquier[clickX-4][clickY].type;
            engine->game.echiquier[clickX-2][clickY].equipe = engine->game.echiquier[clickX-4][clickY].equipe;
            if(changeBackground == true) engine->game.echiquier[clickX-2][clickY].background = engine->game.echiquier[clickX-4][clickY].background;
            engine->game.echiquier[clickX-2][clickY].nombreDeplacement = 1;
            engine->game.echiquier[clickX-4][clickY].type = EMPTY;
            engine->game.echiquier[clickX-4][clickY].equipe = EMPTY_TEAM;
            if(changeBackground == true) engine->game.echiquier[clickX-4][clickY].background = emptyCase;
        }
    }


    //on met � jour la nouvelle position de la pi�ce
    engine->game.echiquier[clickXFinal][clickYFinal].type = engine->game.echiquier[clickX][clickY].type;
    engine->game.echiquier[clickXFinal][clickYFinal].equipe = getMonEquipe(engine);
    if(changeBackground == true) engine->game.echiquier[clickXFinal][clickYFinal].background = engine->game.echiquier[clickX][clickY].background;
    engine->game.echiquier[clickXFinal][clickYFinal].nombreDeplacement++;

    //pour les pions, on enregistre la taille du d�placement
    if(engine->game.echiquier[clickX][clickY].type == PION)
       engine->game.echiquier[clickXFinal][clickYFinal].lastTailleDeplacement = abs(clickYFinal - clickY);
    else
        engine->game.echiquier[clickXFinal][clickYFinal].lastTailleDeplacement = 1;

    //on enregistre la position du dernier d�placement pour chaque �quipe
    if(engine->game.tourPlayer == WHITE){
        engine->game.lastDeplacementWhiteX = clickXFinal;
        engine->game.lastDeplacementWhiteY = clickYFinal;
    }
    else{
        engine->game.lastDeplacementBlackX = clickXFinal;
        engine->game.lastDeplacementBlackY = clickYFinal;
    }

    //on met � jour l'ancienne position de la pi�ce
    engine->game.echiquier[clickX][clickY].type = EMPTY;
    engine->game.echiquier[clickX][clickY].equipe = EMPTY_TEAM;
    if(changeBackground == true) engine->game.echiquier[clickX][clickY].background = emptyCase;

    engine->game.nombreDeplacement++;

    delete [] cstr;
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param int clickXFinal : coordonn�e X de la case d'arriv�e
* @param int clickYFinal : coordonn�e Y de la pi�ce d'arriv�e
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplacePion(Engine * engine, int clickX, int clickY, Game * game){
    if(engine->game.tourPlayer == WHITE){
        if(clickY+1 < WIDTH_ARRAY){
            if(engine->game.echiquier[clickX][clickY+1].equipe == EMPTY_TEAM){ //pas de la m�me equipe
                game->echiquierDeplacement[clickX][clickY+1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickY+2 < WIDTH_ARRAY){
            if(engine->game.echiquier[clickX][clickY+1].equipe == EMPTY_TEAM //pas de la m�me equipe
            && engine->game.echiquier[clickX][clickY+2].equipe == EMPTY_TEAM //d'aucune �quipe
            && engine->game.echiquier[clickX][clickY].nombreDeplacement == 0){ //0 d�placements avant
                game->echiquierDeplacement[clickX][clickY+2] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickX+1 < WIDTH_ARRAY && clickY+1 < WIDTH_ARRAY){
            if(engine->game.echiquier[clickX+1][clickY+1].equipe == getEquipeAdverse(engine)){
                game->echiquierDeplacement[clickX+1][clickY+1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickX-1 >= 0 && clickY+1 < WIDTH_ARRAY){
            if(engine->game.echiquier[clickX-1][clickY+1].equipe == getEquipeAdverse(engine)){
                game->echiquierDeplacement[clickX-1][clickY+1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }
    }
    else{
        if(clickY-1 >= 0){
            if(engine->game.echiquier[clickX][clickY-1].equipe == EMPTY_TEAM){ //pas de la m�me equipe
                game->echiquierDeplacement[clickX][clickY-1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickY-2 >= 0){
            if(engine->game.echiquier[clickX][clickY-1].equipe == EMPTY_TEAM //d'aucune �quipe
            && engine->game.echiquier[clickX][clickY-2].equipe == EMPTY_TEAM //d'aucune �quipe
            && engine->game.echiquier[clickX][clickY].nombreDeplacement == 0){ //0 d�placements avant
                game->echiquierDeplacement[clickX][clickY-2] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickX+1 < WIDTH_ARRAY && clickY-1 >= 0){
            if(engine->game.echiquier[clickX+1][clickY-1].equipe == getEquipeAdverse(engine)){
                game->echiquierDeplacement[clickX+1][clickY-1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }

        if(clickX-1 >= 0 && clickY-1 >= 0){
            if(engine->game.echiquier[clickX-1][clickY-1].equipe == getEquipeAdverse(engine)){
                game->echiquierDeplacement[clickX-1][clickY-1] = 1;
                engine->game.nombreDeplacementPossible++;
            }
        }
    }
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplaceCavalier(Engine * engine, int clickX, int clickY, Game * game){
    //dans le sens des aiguilles d'une montre � partir de gauche-bas
    if(clickX-1 >= 0 && clickY-2 >= 0){
        if(engine->game.echiquier[clickX-1][clickY-2].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-1][clickY-2] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX-2 >= 0 && clickY-1 >= 0){
        if(engine->game.echiquier[clickX-2][clickY-1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-2][clickY-1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX-2 >= 0 && clickY+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX-2][clickY+1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-2][clickY+1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX-1 >= 0 && clickY+2 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX-1][clickY+2].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-1][clickY+2] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+1 < WIDTH_ARRAY && clickY+2 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX+1][clickY+2].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+1][clickY+2] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+2 < WIDTH_ARRAY && clickY+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX+2][clickY+1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+2][clickY+1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+2 < WIDTH_ARRAY && clickY-1 >= 0){
        if(engine->game.echiquier[clickX+2][clickY-1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+2][clickY-1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+1 < WIDTH_ARRAY && clickY-2 >= 0){
        if(engine->game.echiquier[clickX+1][clickY-2].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+1][clickY-2] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplaceFou(Engine * engine, int clickX, int clickY, Game * game){
    int i,j;
    bool trajectoireBloquee = false;

    //on fait les quatres diagonales en partant de celle de gauche-bas et dans le sens des aiguilles d'une montre
    i = clickX-1;
    j = clickY-1;

    while(trajectoireBloquee == false && i >= 0 && j >= 0){
        if(engine->game.echiquier[i][j].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][j] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][j].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }

        i--;
        j--;
    }

    trajectoireBloquee = false;

    i = clickX-1;
    j = clickY+1;

    while(trajectoireBloquee == false && i >= 0 && j < WIDTH_ARRAY){
        if(engine->game.echiquier[i][j].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][j] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][j].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }

        i--;
        j++;
    }

    trajectoireBloquee = false;

    i = clickX+1;
    j = clickY+1;

    while(trajectoireBloquee == false && i < WIDTH_ARRAY && j < WIDTH_ARRAY){
        if(engine->game.echiquier[i][j].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][j] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][j].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }

        i++;
        j++;
    }

    trajectoireBloquee = false;

    i = clickX+1;
    j = clickY-1;

    while(trajectoireBloquee == false && i < WIDTH_ARRAY && j >= 0){
        if(engine->game.echiquier[i][j].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][j] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][j].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }

        i++;
        j--;
    }
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplaceTour(Engine * engine, int clickX, int clickY, Game * game){
    int i;
    bool trajectoireBloquee = false;

    //a gauche
    for(i = clickX-1; i >= 0 && trajectoireBloquee == false; i--){
        //si la case a c�te n'est pas une case amie
        if(engine->game.echiquier[i][clickY].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][clickY] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][clickY].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }
    }

    trajectoireBloquee = false;

    //a droite
    for(i = clickX+1; i < WIDTH_ARRAY && trajectoireBloquee == false; i++){
        //si la case a c�te n'est pas une case amie
        if(engine->game.echiquier[i][clickY].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[i][clickY] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
             trajectoireBloquee = true;
        }

        if(engine->game.echiquier[i][clickY].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }
    }

    trajectoireBloquee = false;

    //en bas
    for(i = clickY-1; i >= 0 && trajectoireBloquee == false; i--){
        //si la case a c�te n'est pas une case amie
        if(engine->game.echiquier[clickX][i].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX][i] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[clickX][i].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }
    }

    trajectoireBloquee = false;

    //en haut
    for(i = clickY+1; i < WIDTH_ARRAY && trajectoireBloquee == false; i++){
        //si la case a c�te n'est pas une case amie
        if(engine->game.echiquier[clickX][i].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX][i] = 1;
            engine->game.nombreDeplacementPossible++;
        }
        else{
            trajectoireBloquee = true;
        }

        if(engine->game.echiquier[clickX][i].equipe == getEquipeAdverse(engine)){
            trajectoireBloquee = true;
        }
    }
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplaceDame(Engine * engine, int clickX, int clickY, Game * game){
    deplaceTour(engine, clickX, clickY, game);
    deplaceFou(engine, clickX, clickY, game);
}

/**
* calcule d�placement piece
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param Game * game : contient en partie l'�chiquier
* @access public
* @return void
*/
void deplaceRoi(Engine * engine, int clickX, int clickY, Game * game){
    //dans le sens des aiguilles d'une montre � partir de gauche-bas
    if(clickX-1 >= 0 && clickY-1 >= 0){
        if(engine->game.echiquier[clickX-1][clickY-1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-1][clickY-1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX-1 >= 0){
        if(engine->game.echiquier[clickX-1][clickY].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-1][clickY] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX-1 >= 0 && clickY+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX-1][clickY+1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX-1][clickY+1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickY+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX][clickY+1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX][clickY+1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+1 < WIDTH_ARRAY && clickY+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX+1][clickY+1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+1][clickY+1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+1 < WIDTH_ARRAY){
        if(engine->game.echiquier[clickX+1][clickY].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+1][clickY] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickX+1 < WIDTH_ARRAY && clickY-1 >= 0){
        if(engine->game.echiquier[clickX+1][clickY-1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX+1][clickY-1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }

    if(clickY-1 >= 0){
        if(engine->game.echiquier[clickX][clickY-1].equipe != getMonEquipe(engine)){
            game->echiquierDeplacement[clickX][clickY-1] = 1;
            engine->game.nombreDeplacementPossible++;
        }
    }
}

/**
* check une promotion apr�s un d�placement
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param int clickXFinal : coordonn�e X de la case d'arriv�e
* @param int clickYFinal : coordonn�e Y de la pi�ce d'arriv�e
* @access public
* @return void
*/
void checkPromotion(Engine * engine, int clickXFinal, int clickYFinal){
    if(engine->game.echiquier[clickXFinal][clickYFinal].type == PION
    && ((clickYFinal == WIDTH_ARRAY-1 && engine->game.tourPlayer == WHITE) || (clickYFinal == 0 && engine->game.tourPlayer == BLACK))){
        SDL_Event event;
        PositionItem position;
        int clickId;
        bool finished = false;

        drawMenuPromotion(engine);

        while(finished == false){
            SDL_WaitEvent(&event);
            switch(event.type){
                case SDL_QUIT:
                    engine->currentAction = QUIT;
                    finished = true;
                break;

                case SDL_MOUSEBUTTONUP:
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menuPromotion.menu, NOMBRE_ITEM_MENU_PROMOTION, position);

                    switch(clickId){
                        case GAME_MENU_PROMOTION_FOU :
                            engine->game.echiquier[clickXFinal][clickYFinal].type = FOU;

                            if(engine->game.tourPlayer == WHITE)
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_FOU_WHITE);
                            else
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_FOU_BLACK);

                            finished = true;
                            engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une promotion
                        break;

                        case GAME_MENU_PROMOTION_TOUR :
                            engine->game.echiquier[clickXFinal][clickYFinal].type = TOUR;

                            if(engine->game.tourPlayer == WHITE)
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_TOUR_WHITE);
                            else
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_TOUR_BLACK);

                            finished = true;
                            engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une promotion
                        break;

                        case GAME_MENU_PROMOTION_CAVALIER :
                            engine->game.echiquier[clickXFinal][clickYFinal].type = CAVALIER;

                            if(engine->game.tourPlayer == WHITE)
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_CAVALIER_WHITE);
                            else
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_CAVALIER_BLACK);

                            finished = true;
                            engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une promotion
                        break;

                        case GAME_MENU_PROMOTION_REINE :
                            engine->game.echiquier[clickXFinal][clickYFinal].type = DAME;

                            if(engine->game.tourPlayer == WHITE)
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_DAME_WHITE);
                            else
                                engine->game.echiquier[clickXFinal][clickYFinal].background = IMG_Load(SPRITE_DAME_BLACK);

                            finished = true;
                            engine->game.nombreCoupLastCapturePromotion = engine->game.nombreDeplacement; //on stocke le n-i�me d�placement quand y a eu une promotion
                        break;
                    }
                break;
            }
        }
    }
}

/**
* check la prise en passant
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param int clickXFinal : coordonn�e X de la case d'arriv�e
* @param int clickYFinal : coordonn�e Y de la pi�ce d'arriv�e
* @access public
* @return void
*/
void checkPriseEnPassant(Engine * engine, int clickX, int clickY){
    //blancs : y = 5, noirs : y = 3 //on regarde si  la pi�ce est un pion
    if(((engine->game.echiquier[clickX][clickY].equipe == WHITE && clickY == 5-1)
    || (engine->game.echiquier[clickX][clickY].equipe == BLACK && clickY == 4-1)) && engine->game.echiquier[clickX][clickY].type == PION){
        //on regarde si les cases � c�t� contiennent un pion //on regarde si le pion s'est d�plac� une seule fois de 2 cases // si adverse
        if(engine->game.echiquier[clickX-1][clickY].type == PION
        && engine->game.echiquier[clickX-1][clickY].lastTailleDeplacement == 2
        && engine->game.echiquier[clickX-1][clickY].equipe == getEquipeAdverse(engine)){
            if(engine->game.echiquier[clickX][clickY].equipe == WHITE
            && engine->game.lastDeplacementBlackX == clickX-1 && engine->game.lastDeplacementBlackY == clickY){
                engine->game.echiquierDeplacement[clickX-1][clickY+1] = D_POSSIBLE;
                engine->game.echiquierDeplacement[clickX-1][clickY] = D_PRISE_EN_PASSANT; //On doit supprimer la pi�ce ici
                engine->game.nombreDeplacementPossible++;
            }
            else if(engine->game.echiquier[clickX][clickY].equipe == BLACK
            && engine->game.lastDeplacementWhiteX == clickX-1 && engine->game.lastDeplacementWhiteY == clickY){
                engine->game.echiquierDeplacement[clickX-1][clickY-1] = D_POSSIBLE;
                engine->game.echiquierDeplacement[clickX-1][clickY] = D_PRISE_EN_PASSANT; //On doit supprimer la pi�ce ici
                engine->game.nombreDeplacementPossible++;
            }
        }
        //on regarde si les cases � c�t� contiennent un pion //on regarde si le pion s'est d�plac� une seule fois de 2 cases
        if(engine->game.echiquier[clickX+1][clickY].type == PION
        && engine->game.echiquier[clickX+1][clickY].lastTailleDeplacement == 2
        && engine->game.echiquier[clickX+1][clickY].equipe == getEquipeAdverse(engine)){
            if(engine->game.echiquier[clickX][clickY].equipe == WHITE
            && engine->game.lastDeplacementBlackX == clickX+1 && engine->game.lastDeplacementBlackY == clickY){
                engine->game.echiquierDeplacement[clickX+1][clickY+1] = D_POSSIBLE;
                engine->game.echiquierDeplacement[clickX+1][clickY] = D_PRISE_EN_PASSANT; //On doit supprimer la pi�ce ici
                engine->game.nombreDeplacementPossible++;
            }
            else if(engine->game.echiquier[clickX][clickY].equipe == BLACK
            && engine->game.lastDeplacementWhiteX == clickX+1 && engine->game.lastDeplacementWhiteY == clickY){
                engine->game.echiquierDeplacement[clickX+1][clickY-1] = D_POSSIBLE;
                engine->game.echiquierDeplacement[clickX+1][clickY] = D_PRISE_EN_PASSANT; //On doit supprimer la pi�ce ici
                engine->game.nombreDeplacementPossible++;
            }
        }
    }
}

/**
* check le roque
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @param int clickXFinal : coordonn�e X de la case d'arriv�e
* @param int clickYFinal : coordonn�e Y de la pi�ce d'arriv�e
* @access public
* @return void
*/
void checkRoque(Engine * engine, int clickX, int clickY){
    //on a boug� le roi pour la premi�re fois, sur la m�me rang�e
    if(engine->game.echiquier[clickX][clickY].type == ROI
    && engine->game.echiquier[clickX][clickY].nombreDeplacement == 0){
        //si la case finale est � c�t� de la tour de droite et que la tour n'a pas encore boug�
        //si les cases entre sont libres
        if(clickX+3 < WIDTH_ARRAY
        && engine->game.echiquier[clickX+3][clickY].type == TOUR
        && engine->game.echiquier[clickX+3][clickY].nombreDeplacement == 0
        && engine->game.echiquier[clickX+1][clickY].type == EMPTY
        && engine->game.echiquier[clickX+2][clickY].type == EMPTY){
            engine->game.echiquierDeplacement[clickX+2][clickY] = D_POSSIBLE;
            engine->game.echiquierDeplacement[clickX+3][clickY] = D_ROQUE;
            engine->game.nombreDeplacementPossible++;
            engine->game.roiRoquePossible = true;
        }
        //si la case finale est � c�t� de la tour de gauche et que la tour n'a pas encore boug�
        //si les cases entre sont libres
        if(clickX-4 >= 0
        && engine->game.echiquier[clickX-4][clickY].type == TOUR
        && engine->game.echiquier[clickX-4][clickY].nombreDeplacement == 0
        && engine->game.echiquier[clickX-1][clickY].type == EMPTY
        && engine->game.echiquier[clickX-2][clickY].type == EMPTY
        && engine->game.echiquier[clickX-3][clickY].type == EMPTY){
            engine->game.echiquierDeplacement[clickX-2][clickY] = D_ROQUE_CASE_TRAVERSEE;
            engine->game.echiquierDeplacement[clickX-3][clickY] = D_POSSIBLE;
            engine->game.echiquierDeplacement[clickX-4][clickY] = D_ROQUE;
            engine->game.nombreDeplacementPossible++;
            engine->game.roiRoquePossible = true;
        }
    }
}

/**
* check la mise en �chec du roi. Pour cela, calcul quelles sont les positions qui sont possibles en tenant compte de la mise en �chec
* @param Engine * engine : pointeur vers la structure principale
* @param int clickX : coordonn�e X de la pi�ce
* @param int clickY : coordonn�e Y de la pi�ce
* @access public
* @return bool
*/
bool checkEchec(Engine * engine, int clickX, int clickY){
    //on cr�� une structure engine pour les d�placements des amis et une autre pour les ennemis
    PositionItem positionRoi;
    Engine engineCalculAmi;
    Engine engineCalculEnnemi;
    bool enEchec = false;

    trouverRoi(engine, &positionRoi);

    //dans le cas o� on veut jouer le roi pour le roque, on regarde si il peut r�aliser un des roques.
    //Si il peut r�aliser un roque mais qu'il est en �chec, alors, on bloque le roque
    if(engine->game.roiRoquePossible == true){
        engineCalculAmi = (*engine);
        clearTabDeplacement(&engineCalculAmi);
        //pour chaque piece ennemie, on regarde si elle met en echec le roi
        for(int k = 0; k < WIDTH_ARRAY; k++){
            for(int l = 0; l < WIDTH_ARRAY; l++){
                if(engineCalculAmi.game.echiquier[k][l].equipe == getEquipeAdverse(engine)){
                    engineCalculEnnemi = (engineCalculAmi);
                    engineCalculEnnemi.game.tourPlayer = getEquipeAdverse(engine);

                    clearTabDeplacement(&engineCalculEnnemi);

                    calculDeplacementPiece(&engineCalculEnnemi, k, l);
                    checkPriseEnPassant(&engineCalculEnnemi, k, l);

                    //maintenant qu'on a calcul� les d�placements possibles, on cherche � savoir si la case du roi est mise en �chec
                    if(engineCalculEnnemi.game.echiquierDeplacement[positionRoi.x][positionRoi.y] == D_MISE_EN_ECHEC){
                        engine->game.echiquierDeplacement[positionRoi.x+3][positionRoi.y] = D_IMPOSSIBLE;
                        engine->game.echiquierDeplacement[positionRoi.x-2][positionRoi.y] = D_IMPOSSIBLE;
                        engine->game.echiquierDeplacement[positionRoi.x-4][positionRoi.y] = D_IMPOSSIBLE;
                        enEchec = true;

                        if(engine->game.echiquierDeplacement[positionRoi.x-3][positionRoi.y] == D_POSSIBLE){
                            engine->game.nombreDeplacementPossible--;
                            engine->game.echiquierDeplacement[positionRoi.x-3][positionRoi.y] = D_IMPOSSIBLE;
                        }

                        if(engine->game.echiquierDeplacement[positionRoi.x+2][positionRoi.y] == D_POSSIBLE){
                            engine->game.nombreDeplacementPossible--;
                            engine->game.echiquierDeplacement[positionRoi.x+2][positionRoi.y] = D_IMPOSSIBLE;
                        }
                    }
                }
            }
        }
    }

    //pour chacune des cases o� l'on peut jouer, v�rifier si en d�pla�ant la pi�ce cela ne provoque pas une mise en echec du roi
    //on va donc simuler � chaque fois le d�placement
    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            //s'il s'agit d'une case d�pla�able ou d'une case travers�e pendant le roque
            if(engine->game.echiquierDeplacement[i][j] == D_POSSIBLE || engine->game.echiquierDeplacement[i][j] == D_ROQUE_CASE_TRAVERSEE){
                engineCalculAmi = (*engine);
                clearTabDeplacement(&engineCalculAmi);
                deplacePiece(&engineCalculAmi, clickX, clickY, i, j); //on d�place la pi�ce a un endroit possible

                //si la pi�ce est le roi, on doit modifier la position enregistr�e du roi vu qu'il a boug�
                if(engine->game.echiquier[clickX][clickY].type == ROI){
                    positionRoi.x = i;
                    positionRoi.y = j;
                }

                //ensuite, pour chaque pi�ce ennemie, on cherche � savoir si elle peut attaquer le roi apr�s le d�placement
                for(int k = 0; k < WIDTH_ARRAY; k++){
                    for(int l = 0; l < WIDTH_ARRAY; l++){
                        if(engineCalculAmi.game.echiquier[k][l].equipe == getEquipeAdverse(engine)){
                            engineCalculEnnemi = (engineCalculAmi);
                            engineCalculEnnemi.game.tourPlayer = getEquipeAdverse(engine);

                            clearTabDeplacement(&engineCalculEnnemi);

                            calculDeplacementPiece(&engineCalculEnnemi, k, l);
                            checkPriseEnPassant(&engineCalculEnnemi, k, l);
                            //pas de roque car le roque n'est pas une menace pour le roi adverse

                            //maintenant qu'on a calcul� les d�placements possibles, on cherche � savoir si la case du roi est mise en �chec
                            if(engineCalculEnnemi.game.echiquierDeplacement[positionRoi.x][positionRoi.y] == D_MISE_EN_ECHEC){
                                engine->game.echiquierDeplacement[i][j] = D_IMPOSSIBLE;
                                enEchec = true;
                                engine->game.nombreDeplacementPossible--;
                            }
                        }
                    }
                }
            }
        }
    }

    engine->game.enEchec = enEchec;
    return enEchec;
}

/**
* check la fin de la partie par �chec et mat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return bool
*/
bool checkEchecEtMat(Engine * engine){
    bool enEchec = false;
    bool enEchecEtMat = true;
    Engine engineCalculAmi = (*engine); //on doit travailler sur une copie pour ne pas bousiller l'original
    Engine engineCalculEnnemi = (*engine);
    PositionItem positionRoi;

    trouverRoi(engine, &positionRoi);

    engineCalculEnnemi.game.tourPlayer = getEquipeAdverse(&engineCalculEnnemi);
    clearTabDeplacement(&engineCalculEnnemi);
    clearTabDeplacement(&engineCalculAmi);

    //il faut savoir si l'adversaire a mis en echec le joueur lors du tour pr�c�dent
    //on teste donc tous les d�placements possible de l'ennemi pour trouver une mise en �chec
    for(int i = 0; i < WIDTH_ARRAY && enEchec == false; i++){
        for(int j = 0; j < WIDTH_ARRAY && enEchec == false; j++){
            if(engineCalculEnnemi.game.echiquier[i][j].equipe == getMonEquipe(&engineCalculEnnemi)){
                clearTabDeplacement(&engineCalculEnnemi);
                calculDeplacementPiece(&engineCalculEnnemi, i, j);
                checkPriseEnPassant(&engineCalculEnnemi, i, j);
                checkRoque(&engineCalculEnnemi, i, j);

                //on regarde si le roi est en �chec
                if(engineCalculEnnemi.game.echiquierDeplacement[positionRoi.x][positionRoi.y] == D_MISE_EN_ECHEC){
                     enEchec = true;
                }
            }
        }
    }

    //si jamais le joueur a �t� mis en �chec, on doit savoir si il peut faire une d�placement qui prot�ge le roi
    //pour cela, on va tester tous les d�placements de toutes les pi�ces et voir si une pi�ce au moins poss�de un d�placement
    //dans ce cas, le joueur n'est pas encore echec et mat
    if(enEchec == true){
        for(int i = 0; i < WIDTH_ARRAY; i++){
            for(int j = 0; j < WIDTH_ARRAY; j++){
                engineCalculAmi.game.nombreDeplacementPossible = 0;
                if(engineCalculAmi.game.echiquier[i][j].equipe == getMonEquipe(&engineCalculAmi)){
                    clearTabDeplacement(&engineCalculAmi);
                    calculDeplacementPiece(&engineCalculAmi, i, j);
                    checkPriseEnPassant(&engineCalculAmi, i, j);
                    checkRoque(&engineCalculAmi, i, j);
                    checkEchec(&engineCalculAmi, i, j);

                    //on regarde si le pion peut faire un d�placement qui sauve le roi
                    if(engineCalculAmi.game.nombreDeplacementPossible > 0){
                         enEchecEtMat = false;
                    }
                }
            }
        }
    }
    else{
        enEchecEtMat = false;engine->game.enEchecEtMat = false;
    }

    engine->game.enEchecEtMat = enEchecEtMat;
    return enEchecEtMat;
}

/**
* check le pat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return bool
*/
bool checkPat(Engine * engine){
    bool enEchec = false;
    bool enPat = true;

    Engine engineCalculAmi = (*engine); //on doit travailler sur une copie pour ne pas bousiller l'original
    Engine engineCalculEnnemi = (*engine);
    PositionItem positionRoi;

    trouverRoi(engine, &positionRoi);

    engineCalculEnnemi.game.tourPlayer = getEquipeAdverse(&engineCalculEnnemi);
    clearTabDeplacement(&engineCalculEnnemi);
    clearTabDeplacement(&engineCalculAmi);

    //il faut savoir si l'adversaire a mis en echec le joueur lors du tour pr�c�dent
    //on teste donc tous les d�placements possible de l'ennemi pour trouver une mise en �chec
    for(int i = 0; i < WIDTH_ARRAY && enEchec == false; i++){
        for(int j = 0; j < WIDTH_ARRAY && enEchec == false; j++){
            if(engineCalculEnnemi.game.echiquier[i][j].equipe == getMonEquipe(&engineCalculEnnemi)){
                clearTabDeplacement(&engineCalculEnnemi);
                calculDeplacementPiece(&engineCalculEnnemi, i, j);
                checkPriseEnPassant(&engineCalculEnnemi, i, j);
                checkRoque(&engineCalculEnnemi, i, j);

                //on regarde si le roi est en �chec
                if(engineCalculEnnemi.game.echiquierDeplacement[positionRoi.x][positionRoi.y] == D_MISE_EN_ECHEC){
                     enEchec = true;
                }
            }
        }
    }

    //si jamais le joueur a �t� mis en �chec, on doit savoir si il peut faire une d�placement qui prot�ge le roi
    //pour cela, on va tester tous les d�placements de toutes les pi�ces et voir si une pi�ce au moins poss�de un d�placement
    //dans ce cas, le joueur n'est pas encore echec et mat
    if(enEchec == false){
        for(int i = 0; i < WIDTH_ARRAY; i++){
            for(int j = 0; j < WIDTH_ARRAY; j++){
                engineCalculAmi.game.nombreDeplacementPossible = 0;
                if(engineCalculAmi.game.echiquier[i][j].equipe == getMonEquipe(&engineCalculAmi)){
                    clearTabDeplacement(&engineCalculAmi);
                    calculDeplacementPiece(&engineCalculAmi, i, j);
                    checkPriseEnPassant(&engineCalculAmi, i, j);
                    checkRoque(&engineCalculAmi, i, j);
                    checkEchec(&engineCalculAmi, i, j);

                    //on regarde si le pion peut faire un d�placement qui sauve le roi
                    if(engineCalculAmi.game.nombreDeplacementPossible > 0){
                         enPat = false;
                    }
                }
            }
        }
    }
    else{
        enPat = false;
        engine->game.enPat = false;
    }

    engine->game.enPat = enPat;
    return enPat;
}

/**
* affiche le menu
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void menuEchecEtMat(Engine * engine){
    SDL_Event event;
    bool finished =false;
    PositionItem position;
    int clickId;

    drawMenuEchecEtMat(engine);

    while(finished == false){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                finished = true;
                engine->currentAction = QUIT;
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menuEchecEtMat.menu, NOMBRE_ITEM_MENU_ECHEC_ET_MAT, position);

                    if(clickId != ERROR){
                        if(clickId == 0){
                            engine->currentAction = NEWGAMEAGAIN;
                        }
                        else{
                            engine->currentAction = QUIT;
                        }

                        finished  = true;
                    }
                }
            break;
        }
    }
}

/**
* check le pat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void menuPat(Engine * engine){
    SDL_Event event;
    bool finished = false;
    PositionItem position;
    int clickId;

    drawMenuPat(engine);

    while(finished == false){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                finished = true;
                engine->currentAction = QUIT;
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menuPat.menu, NOMBRE_ITEM_MENU_PAT, position);

                    if(clickId != ERROR){
                        if(clickId == 0){
                            engine->currentAction = NEWGAMEAGAIN;
                        }
                        else{
                            engine->currentAction = QUIT;
                        }

                        finished  = true;
                    }
                }
            break;
        }
    }
}


/**
* g�re un match nul
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void menuMatchNul(Engine * engine){
    SDL_Event event;
    bool finished = false;
    PositionItem position;
    int clickId;

    drawMenuMatchNul(engine);

    while(finished == false){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                finished = true;
                engine->currentAction = QUIT;
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menuMatchNul.menu, NOMBRE_ITEM_MENU_MATCH_NUL, position);

                    if(clickId != ERROR){
                        if(clickId == 0){
                            engine->currentAction = NEWGAMEAGAIN;
                        }
                        else{
                            engine->currentAction = QUIT;
                        }

                        finished  = true;
                    }
                }
            break;
        }
    }
}


/**
* affiche le menu des 50 coups. En cas de r�ponse affirmative, encha�ne sur le menu des parties nulles
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
bool menu50Coup(Engine * engine){
    SDL_Event event;
    bool finished = false;
    bool finishedParty = false;
    PositionItem position;
    int clickId;

    drawMenu50Coup(engine);

    while(finished == false){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                finished = true;
                engine->currentAction = QUIT;
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menu50Coup.menu, NOMBRE_ITEM_MENU_50COUP, position);

                    if(clickId != ERROR){
                        if(clickId == 0){
                            finishedParty = true;
                            menuMatchNul(engine);
                        }
                        else{
                            finishedParty = false;
                        }

                        finished  = true;
                    }
                }
            break;
        }
    }

    return finishedParty;
}


/**
* affiche le menu des 3 coups. En cas de r�ponse affirmative, encha�ne sur le menu des parties nulles
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void menu3Coup(Engine * engine){
    SDL_Event event;
    bool finished = false;
    PositionItem position;
    int clickId;

    drawMenu3Coup(engine);

    while(finished == false){
        SDL_WaitEvent(&event);
        switch(event.type){
            case SDL_QUIT:
                finished = true;
                engine->currentAction = QUIT;
            break;

            case SDL_MOUSEBUTTONUP:
                if(event.button.button == SDL_BUTTON_LEFT){
                    position.x = event.button.x;
                    position.y = event.button.y;

                    clickId = checkClickMenu(engine->game.menu3Coup.menu, NOMBRE_ITEM_MENU_3COUP, position);

                    if(clickId != ERROR){
                        if(clickId == 0){
                            engine->currentAction = NEWGAMEAGAIN;
                        }
                        else{
                            engine->currentAction = QUIT;
                        }

                        finished  = true;
                    }
                }
            break;
        }
    }
}
/**
* trouve le roi de l'�quipe en train de jouer
* @param Engine * engine : pointeur vers la structure principale
* @param PositionItem *position : pointeur vers la structure o� il faudra enregistrer la position
* @access public
* @return void
*/
void trouverRoi(Engine * engine, PositionItem * position){
    bool roiTrouve = false;

    for(int i = 0; i < WIDTH_ARRAY && roiTrouve == false; i++){
        for(int j = 0; j < WIDTH_ARRAY && roiTrouve == false; j++){
            if(engine->game.echiquier[i][j].type == ROI && engine->game.echiquier[i][j].equipe == getMonEquipe(engine)){
                position->x = i;
                position->y = j;
                roiTrouve = true;
            }
        }
    }
}
