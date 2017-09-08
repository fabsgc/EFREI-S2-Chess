#include "function.hpp"

/**
* initialise le plateau de jeu, les pièces
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initGame(Engine * engine){
    engine->game.background = IMG_Load(SPRITE_GAME);
    engine->game.tourPlayer = 0;
    engine->game.positionBackground.x = 0;
    engine->game.positionBackground.y = 0;
    engine->currentAction = NEWGAME;
    engine->game.player[0].win = ENCOURS;
    engine->game.player[1].win = ENCOURS;
    engine->game.nombreTaken = 0;
    engine->game.nombreDeplacement = 0;
    engine->game.enEchec = false;
    engine->game.enEchecEtMat = false;
    engine->game.enPat = false;
    engine->game.lastDeplacementWhiteX = -1;
    engine->game.lastDeplacementWhiteY = -1;
    engine->game.lastDeplacementBlackX = -1;
    engine->game.lastDeplacementBlackY = -1;
    engine->game.demandeRegle50Coup = false;
    engine->game.nombreCoupLastCapturePromotion = 0;
    engine->game.roiRoquePossible = false;

    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            initPiece(&engine->game.echiquier[i][j],  EMPTY, EMPTY_TEAM, SPRITE_EMPTY, i, j);
            engine->game.echiquierDeplacement[i][j] = 0;
        }
    }

    initPiece(&engine->game.echiquier[0][0], TOUR, WHITE, SPRITE_TOUR_WHITE, 0, 0);
    initPiece(&engine->game.echiquier[1][0], CAVALIER, WHITE, SPRITE_CAVALIER_WHITE, 1, 0);
    initPiece(&engine->game.echiquier[2][0], FOU, WHITE, SPRITE_FOU_WHITE, 2, 0);
    initPiece(&engine->game.echiquier[3][0], DAME, WHITE, SPRITE_DAME_WHITE, 3, 0);
    initPiece(&engine->game.echiquier[4][0], ROI, WHITE, SPRITE_ROI_WHITE, 4, 0);
    initPiece(&engine->game.echiquier[5][0], FOU, WHITE, SPRITE_FOU_WHITE, 5, 0);
    initPiece(&engine->game.echiquier[6][0], CAVALIER, WHITE, SPRITE_CAVALIER_WHITE, 6, 0);
    initPiece(&engine->game.echiquier[7][0], TOUR, WHITE, SPRITE_TOUR_WHITE, 7, 0);

    initPiece(&engine->game.echiquier[0][1], PION, WHITE, SPRITE_PION_WHITE, 0, 1);
    initPiece(&engine->game.echiquier[1][1], PION, WHITE, SPRITE_PION_WHITE, 1, 1);
    initPiece(&engine->game.echiquier[2][1], PION, WHITE, SPRITE_PION_WHITE, 2, 1);
    initPiece(&engine->game.echiquier[3][1], PION, WHITE, SPRITE_PION_WHITE, 3, 1);
    initPiece(&engine->game.echiquier[4][1], PION, WHITE, SPRITE_PION_WHITE, 4, 1);
    initPiece(&engine->game.echiquier[5][1], PION, WHITE, SPRITE_PION_WHITE, 5, 1);
    initPiece(&engine->game.echiquier[6][1], PION, WHITE, SPRITE_PION_WHITE, 6, 1);
    initPiece(&engine->game.echiquier[7][1], PION, WHITE, SPRITE_PION_WHITE, 7, 1);

    initPiece(&engine->game.echiquier[0][7], TOUR, BLACK, SPRITE_TOUR_BLACK, 0, 7);
    initPiece(&engine->game.echiquier[1][7], CAVALIER, BLACK, SPRITE_CAVALIER_BLACK, 1, 7);
    initPiece(&engine->game.echiquier[2][7], FOU, BLACK, SPRITE_FOU_BLACK, 2, 7);
    initPiece(&engine->game.echiquier[3][7], DAME, BLACK, SPRITE_DAME_BLACK, 3, 7);
    initPiece(&engine->game.echiquier[4][7], ROI, BLACK, SPRITE_ROI_BLACK, 4, 7);
    initPiece(&engine->game.echiquier[5][7], FOU, BLACK, SPRITE_FOU_BLACK, 5, 7);
    initPiece(&engine->game.echiquier[6][7], CAVALIER, BLACK, SPRITE_CAVALIER_BLACK, 6, 7);
    initPiece(&engine->game.echiquier[7][7], TOUR, BLACK, SPRITE_TOUR_BLACK, 7, 7);

    initPiece(&engine->game.echiquier[0][6], PION, BLACK, SPRITE_PION_BLACK, 0, 6);
    initPiece(&engine->game.echiquier[1][6], PION, BLACK, SPRITE_PION_BLACK, 1, 6);
    initPiece(&engine->game.echiquier[2][6], PION, BLACK, SPRITE_PION_BLACK, 2, 6);
    initPiece(&engine->game.echiquier[3][6], PION, BLACK, SPRITE_PION_BLACK, 3, 6);
    initPiece(&engine->game.echiquier[4][6], PION, BLACK, SPRITE_PION_BLACK, 4, 6);
    initPiece(&engine->game.echiquier[5][6], PION, BLACK, SPRITE_PION_BLACK, 5, 6);
    initPiece(&engine->game.echiquier[6][6], PION, BLACK, SPRITE_PION_BLACK, 6, 6);
    initPiece(&engine->game.echiquier[7][6], PION, BLACK, SPRITE_PION_BLACK, 7, 6);

    engine->game.message =  "";
    engine->game.couleurMessage.r = 0;
    engine->game.couleurMessage.g = 0;
    engine->game.couleurMessage.b = 0;
    TTF_Init();
    engine->game.policeMessage = TTF_OpenFont(POLICE, 18);
    engine->game.positionMessage.x = 403;
    engine->game.positionMessage.y = 12;

    engine->game.surfaceCaseSelected = IMG_Load(SPRITE_PIECE_SELECTED);
    engine->game.surfaceCaseJouable = IMG_Load(SPRITE_PIECE_JOUABLE);
    engine->game.surfaceCaseJouableEnnemi = IMG_Load(SPRITE_PIECE_JOUABLE_ENNEMI);

    initMenuPromotion(engine);
    initMenuEchecEtMat(engine);
    initMenuPat(engine);
    initMenuMatchNul(engine);
    initMenu50Coup(engine);
    initMenu3Coup(engine);
}

/**
* vide le tableau contenant les cases jouables
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void clearTabDeplacement(Engine * engine){
    for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            engine->game.echiquierDeplacement[i][j] = 0;
        }
    }
}

/**
* affiche un message dans la zone de texte
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void writeMessage(Engine * engine){
    char * cstr = new char[engine->game.message.length() + 1];
    strcpy(cstr, engine->game.message.c_str());

    engine->game.surfaceMessage = TTF_RenderText_Blended(engine->game.policeMessage, cstr, engine->game.couleurMessage);
    SDL_BlitSurface(engine->game.surfaceMessage, NULL, engine->window, &engine->game.positionMessage);
    SDL_FreeSurface(engine->game.surfaceMessage);

    delete [] cstr;
}

/**
* initialise le menu principal du jeu
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenu(Engine * engine){
    engine->menu.background = IMG_Load(SPRITE_MENU);
    engine->menu.positionBackground.x = 0;
    engine->menu.positionBackground.y = 0;

    initMenuItem(&engine->menu.menu[MENU_RETOUR_AU_JEU], MENU_RETOUR_AU_JEU, SPRITE_MENU_RETOUR_AU_JEU, 22, 22 + (HEIGHT_MENU_MENU + 22) * 0, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->menu.menu[MENU_NOUVELLE_PARTIE], MENU_NOUVELLE_PARTIE, SPRITE_MENU_NOUVELLE_PARTIE, 22, 22 + (HEIGHT_MENU_MENU + 22) * 1, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->menu.menu[MENU_ENREGISTRER], MENU_ENREGISTRER, SPRITE_MENU_ENREGISTRER, 22, 22 + (HEIGHT_MENU_MENU + 22) * 2, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->menu.menu[MENU_CHARGER], MENU_CHARGER, SPRITE_MENU_CHARGER, 22, 22 + (HEIGHT_MENU_MENU + 22) * 3, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->menu.menu[MENU_ABANDONNER], MENU_ABANDONNER, SPRITE_MENU_ABANDONNER, 22, 22 + (HEIGHT_MENU_MENU + 22) * 4, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->menu.menu[MENU_QUITTER], MENU_QUITTER, SPRITE_MENU_QUITTER, 22, 22 + (HEIGHT_MENU_MENU + 22) * 5, WIDTH_MENU_MENU, HEIGHT_MENU_MENU);
    initMenuItem(&engine->game.menu[GAME_MENU], GAME_MENU, SPRITE_EMPTY, 56, 10, WIDTH_MENU_MENU, 30);
}

/**
* initialise les items du menu principal
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenuItem(GuiMenuItem * menu, int id, string sprite, int x, int y, int width, int height){
    char * cstr = new char[sprite.length() + 1];
    strcpy(cstr, sprite.c_str());

    menu->background = IMG_Load(cstr);
    menu->position.x = x;
    menu->position.y = y;
    menu->position.width = width;
    menu->position.height = height;
    menu->positionBackground.x = x;
    menu->positionBackground.y = y;

    delete [] cstr;
}

/**
* initialise une piece
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initPiece(Pieces * piece, int type, int equipe, string sprite, int x, int y){
    char * cstr = new char[sprite.length() + 1];
    strcpy(cstr, sprite.c_str());

    piece->id = 0;
    piece->type = type;
    piece->equipe = equipe;
    piece->background = IMG_Load(cstr);
    piece->position.x = WIDTH_LEFT + x*WIDTH_CASE;
    piece->position.y = WIDTH_BOTTOM - y*WIDTH_CASE;
    piece->position.width = WIDTH_CASE;
    piece->position.height = WIDTH_CASE;
    piece->positionBackground.x = WIDTH_LEFT + x*WIDTH_CASE;
    piece->positionBackground.y = WIDTH_BOTTOM - y*WIDTH_CASE;
    piece->lastTailleDeplacement = 0;
    piece->nombreDeplacement = 0;

    delete [] cstr;
}

/**
* libère la mémoire une fois le jeu fermer
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void freeSurface(Engine * engine){
    SDL_FreeSurface(engine->game.background);
    engine->game.background = NULL;
    SDL_FreeSurface(engine->menu.background);
    engine->menu.background = NULL;

    /*for(int i = 0; i < WIDTH_ARRAY; i++){
        for(int j = 0; j < WIDTH_ARRAY; j++){
            SDL_FreeSurface(engine->game.echiquier[i][j].background);
            engine->game.echiquier[i][j].background = NULL;
        }
    }*/

    for(int i = 0; i < engine->game.nombreTaken; i++){
        SDL_FreeSurface(engine->game.taken[i].background);
        engine->game.taken[i].background = NULL;
    }

    for(int i = 0; i < 6; i++){
        SDL_FreeSurface(engine->menu.menu[i].background);
        engine->menu.menu[i].background = NULL;
    }

    SDL_FreeSurface(engine->game.surfaceMessage);
    SDL_FreeSurface(engine->game.surfaceCaseSelected);
    SDL_FreeSurface(engine->game.surfaceCaseJouable);
    SDL_FreeSurface(engine->game.surfaceCaseJouableEnnemi);

    SDL_FreeSurface(engine->game.menuPromotion.backgroundBlack);
    SDL_FreeSurface(engine->game.menuPromotion.backgroundWhite);

    SDL_FreeSurface(engine->game.menuEchecEtMat.backgroundWhite);
    SDL_FreeSurface(engine->game.menuEchecEtMat.backgroundBlack);

    SDL_FreeSurface(engine->game.menuPat.background);

    SDL_FreeSurface(engine->game.menuMatchNul.background);

    SDL_FreeSurface(engine->game.menu50Coup.background);

    SDL_FreeSurface(engine->game.menu3Coup.background);
}

/**
* check sur quel bouton du menu on a appuyé
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return int : case du tableau correspondant au menu cliqué
*/
int checkClickMenu(GuiMenuItem * menu, int tailleUtile, PositionItem positionClick){
    for(int i = 0; i < tailleUtile; i++){
        if(positionClick.x >= menu[i].position.x && positionClick.x <= (menu[i].position.x + menu[i].position.width) &&
           positionClick.y >= menu[i].position.y && positionClick.y <= (menu[i].position.y + menu[i].position.height)){
            return i;
        }
    }

    return ERROR;
}

/**
* initialise le menu des promotions
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenuPromotion(Engine * engine){
    engine->game.menuPromotion.backgroundWhite = IMG_Load(SPRITE_MENU_PROMOTION_WHITE);
    engine->game.menuPromotion.backgroundBlack = IMG_Load(SPRITE_MENU_PROMOTION_BLACK);
    engine->game.menuPromotion.positionBackground.x = 0;
    engine->game.menuPromotion.positionBackground.y = 0;

    engine->game.menuPromotion.menu[0].position.x = 274;
    engine->game.menuPromotion.menu[0].position.y = 295;
    engine->game.menuPromotion.menu[1].position.x = 394;
    engine->game.menuPromotion.menu[1].position.y = 295;
    engine->game.menuPromotion.menu[2].position.x = 514;
    engine->game.menuPromotion.menu[2].position.y = 295;
    engine->game.menuPromotion.menu[3].position.x = 634;
    engine->game.menuPromotion.menu[3].position.y = 295;

    engine->game.menuPromotion.menu[0].position.width = 120;
    engine->game.menuPromotion.menu[0].position.height = 120;
    engine->game.menuPromotion.menu[1].position.width = 120;
    engine->game.menuPromotion.menu[1].position.height = 120;
    engine->game.menuPromotion.menu[2].position.width = 120;
    engine->game.menuPromotion.menu[2].position.height = 120;
    engine->game.menuPromotion.menu[3].position.width = 120;
    engine->game.menuPromotion.menu[3].position.height = 120;
}

/**
* initialise le menu echec et mat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenuEchecEtMat(Engine * engine){
    engine->game.menuEchecEtMat.backgroundWhite = IMG_Load(SPRITE_MENU_ECHEC_ET_MAT_WHITE);
    engine->game.menuEchecEtMat.backgroundBlack = IMG_Load(SPRITE_MENU_ECHEC_ET_MAT_BLACK);
    engine->game.menuEchecEtMat.positionBackground.x = 0;
    engine->game.menuEchecEtMat.positionBackground.y = 0;

    engine->game.menuEchecEtMat.menu[0].position.x = 200;
    engine->game.menuEchecEtMat.menu[0].position.y = 375;
    engine->game.menuEchecEtMat.menu[1].position.x = 572;
    engine->game.menuEchecEtMat.menu[1].position.y = 375;

    engine->game.menuEchecEtMat.menu[0].position.width = 256;
    engine->game.menuEchecEtMat.menu[0].position.height = 40;
    engine->game.menuEchecEtMat.menu[1].position.width = 256;
    engine->game.menuEchecEtMat.menu[1].position.height = 40;
}

/**
* initialise le menu pat
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenuPat(Engine * engine){
    engine->game.menuPat.background = IMG_Load(SPRITE_MENU_PAT);
    engine->game.menuPat.positionBackground.x = 0;
    engine->game.menuPat.positionBackground.y = 0;

    engine->game.menuPat.menu[0].position.x = 200;
    engine->game.menuPat.menu[0].position.y = 375;
    engine->game.menuPat.menu[1].position.x = 572;
    engine->game.menuPat.menu[1].position.y = 375;

    engine->game.menuPat.menu[0].position.width = 256;
    engine->game.menuPat.menu[0].position.height = 40;
    engine->game.menuPat.menu[1].position.width = 256;
    engine->game.menuPat.menu[1].position.height = 40;
}

/**
* initialise le menu 50 coups
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenu50Coup(Engine * engine){
    engine->game.menu50Coup.background = IMG_Load(SPRITE_MENU_50_COUP);
    engine->game.menu50Coup.positionBackground.x = 0;
    engine->game.menu50Coup.positionBackground.y = 0;

    engine->game.menu50Coup.menu[0].position.x = 200;
    engine->game.menu50Coup.menu[0].position.y = 375;
    engine->game.menu50Coup.menu[1].position.x = 572;
    engine->game.menu50Coup.menu[1].position.y = 375;

    engine->game.menu50Coup.menu[0].position.width = 256;
    engine->game.menu50Coup.menu[0].position.height = 40;
    engine->game.menu50Coup.menu[1].position.width = 256;
    engine->game.menu50Coup.menu[1].position.height = 40;
}

/**
* initialise le menu 3 coups
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenu3Coup(Engine * engine){
    engine->game.menu3Coup.background = IMG_Load(SPRITE_MENU_3_COUP);
    engine->game.menu3Coup.positionBackground.x = 0;
    engine->game.menu3Coup.positionBackground.y = 0;

    engine->game.menu3Coup.menu[0].position.x = 200;
    engine->game.menu3Coup.menu[0].position.y = 375;
    engine->game.menu3Coup.menu[1].position.x = 572;
    engine->game.menu3Coup.menu[1].position.y = 375;

    engine->game.menu3Coup.menu[0].position.width = 256;
    engine->game.menu3Coup.menu[0].position.height = 40;
    engine->game.menu3Coup.menu[1].position.width = 256;
    engine->game.menu3Coup.menu[1].position.height = 40;
}

/**
* initialise le menu match nul
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
void initMenuMatchNul(Engine * engine){
    engine->game.menuMatchNul.background = IMG_Load(SPRITE_MENU_MATCH_NUL);
    engine->game.menuMatchNul.positionBackground.x = 0;
    engine->game.menuMatchNul.positionBackground.y = 0;

    engine->game.menuMatchNul.menu[0].position.x = 200;
    engine->game.menuMatchNul.menu[0].position.y = 375;
    engine->game.menuMatchNul.menu[1].position.x = 572;
    engine->game.menuMatchNul.menu[1].position.y = 375;

    engine->game.menuMatchNul.menu[0].position.width = 256;
    engine->game.menuMatchNul.menu[0].position.height = 40;
    engine->game.menuMatchNul.menu[1].position.width = 256;
    engine->game.menuMatchNul.menu[1].position.height = 40;
}

/**
* modifie le curseur du jeu
* @param Engine * engine : pointeur vers la structure principale
* @access public
* @return void
*/
SDL_Cursor * CreateCursor(){
    const char *image[] = {
      /* width height num_colors chars_per_pixel */
      "    32    32        3            1",
      /* colors */
      "X c #000000",
      ". c #ffffff",
      "  c None",
      /* pixels */
      "X                               ",
      "XX                              ",
      "X.X                             ",
      "X..X                            ",
      "X...X                           ",
      "X....X                          ",
      "X.....X                         ",
      "X......X                        ",
      "X.......X                       ",
      "X........X                      ",
      "X.....XXXXX                     ",
      "X..X..X                         ",
      "X.X X..X                        ",
      "XX  X..X                        ",
      "X    X..X                       ",
      "     X..X                       ",
      "      X..X                      ",
      "      X..X                      ",
      "       XX                       ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "                                ",
      "0,0"
    };

  int i, row, col;
  Uint8 data[4*32];
  Uint8 mask[4*32];
  int hot_x, hot_y;

  i = -1;
  for ( row=0; row<32; ++row ) {
    for ( col=0; col<32; ++col ) {
      if ( col % 8 ) {
        data[i] <<= 1;
        mask[i] <<= 1;
      } else {
        ++i;
        data[i] = mask[i] = 0;
      }
      switch (image[4+row][col]) {
        case 'X':
          data[i] |= 0x01;
          mask[i] |= 0x01;
          break;
        case '.':
          mask[i] |= 0x01;
          break;
        case ' ':
          break;
      }
    }
  }
  sscanf(image[4+row], "%d,%d", &hot_x, &hot_y);
  return SDL_CreateCursor(data, mask, 32, 32, hot_x, hot_y);
}
