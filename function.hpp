#ifndef FUNCTION_HPP_INCLUDED
#define FUNCTION_HPP_INCLUDED

#define WIDTH 1028
#define HEIGHT 710

#define ERROR 200795

#define WIDTH_CASE 75
#define WIDTH_LEFT 398
#define WIDTH_LEFT_PERDU 56
#define WIDTH_TOP 78
#define WIDTH_BOTTOM HEIGHT - 32 - WIDTH_CASE
#define WIDTH_ARRAY 8
#define NOMBRE_CASE 64

#define SPRITE_MENU "sprites/menu.png"
#define SPRITE_GAME "sprites/plateau_4.png"
#define SPRITE_PION_WHITE "sprites/pawn_nw.png"
#define SPRITE_PION_BLACK "sprites/pawn_nb.png"
#define SPRITE_CAVALIER_WHITE "sprites/knight_nw.png"
#define SPRITE_CAVALIER_BLACK "sprites/knight_nb.png"
#define SPRITE_FOU_WHITE "sprites/bishop_nw.png"
#define SPRITE_FOU_BLACK "sprites/bishop_nb.png"
#define SPRITE_TOUR_WHITE "sprites/rook_nw.png"
#define SPRITE_TOUR_BLACK "sprites/rook_nb.png"
#define SPRITE_DAME_WHITE "sprites/queen_nw.png"
#define SPRITE_DAME_BLACK "sprites/queen_nb.png"
#define SPRITE_ROI_WHITE "sprites/king_nw.png"
#define SPRITE_ROI_BLACK "sprites/king_nb.png"
#define SPRITE_EMPTY "sprites/empty.png"

#define SPRITE_PIECE_SELECTED "sprites/piece_selected.png"
#define SPRITE_PIECE_JOUABLE "sprites/zone_jouable.png"
#define SPRITE_PIECE_JOUABLE_ENNEMI "sprites/zone_jouable_ennemi.png"

#define SPRITE_MENU_RETOUR_AU_JEU "sprites/menu/retour_au_jeu.png"
#define SPRITE_MENU_NOUVELLE_PARTIE "sprites/menu/nouvelle_partie.png"
#define SPRITE_MENU_ENREGISTRER "sprites/menu/enregistrer.png"
#define SPRITE_MENU_CHARGER "sprites/menu/charger.png"
#define SPRITE_MENU_ENREGISTRER "sprites/menu/enregistrer.png"
#define SPRITE_MENU_ABANDONNER "sprites/menu/abandonner.png"
#define SPRITE_MENU_QUITTER "sprites/menu/quitter.png"
#define SPRITE_MENU_OUI "sprites/menu/oui.png"
#define SPRITE_MENU_NON "sprites/menu/non.png"
#define SPRITE_MENU_PROMOTION_WHITE "sprites/menu/menu_choice_promotion_white.png"
#define SPRITE_MENU_PROMOTION_BLACK "sprites/menu/menu_choice_promotion_black.png"
#define SPRITE_MENU_ECHEC_ET_MAT_WHITE "sprites/menu/menu_choice_echec_et_mat_blanc.png"
#define SPRITE_MENU_ECHEC_ET_MAT_BLACK "sprites/menu/menu_choice_echec_et_mat_noir.png"
#define SPRITE_MENU_PAT "sprites/menu/menu_choice_pat.png"
#define SPRITE_MENU_MATCH_NUL "sprites/menu/menu_choice_math_nul.png"
#define SPRITE_MENU_3_COUP "sprites/menu/menu_choice_3_coup.png"
#define SPRITE_MENU_50_COUP "sprites/menu/menu_choice_50_coup.png"

#define WIDTH_MENU_MENU 256
#define HEIGHT_MENU_MENU 40
#define NOMBRE_ITEM_MENU 6
#define NOMBRE_ITEM_MENU_PROMOTION 4
#define NOMBRE_ITEM_MENU_ECHEC_ET_MAT 2
#define NOMBRE_ITEM_MENU_GAME 1
#define NOMBRE_ITEM_MENU_PAT 2
#define NOMBRE_ITEM_MENU_50COUP 2
#define NOMBRE_ITEM_MENU_3COUP 2
#define NOMBRE_ITEM_MENU_MATCH_NUL 2

#define POLICE "files/font.ttf"

#include <iostream>
#include <string>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <cmath>

using namespace std;

enum ACTION {GAME = 1, MENU = 2, NEWGAME = 3, QUIT = 4, NEWGAMEAGAIN = 5};
enum PIECES {PION = 1, CAVALIER = 2, FOU = 3, TOUR = 4, DAME = 5, ROI = 6, EMPTY = 7};
enum EQUIPE {WHITE = 0, BLACK = 1, EMPTY_TEAM = 2};
enum MENU_MENU {MENU_RETOUR_AU_JEU = 0, MENU_NOUVELLE_PARTIE = 1, MENU_ENREGISTRER  = 2, MENU_CHARGER = 3, MENU_ABANDONNER = 4, MENU_QUITTER = 5};
enum MENU_GAME {GAME_MENU = 0};
enum MENU_PROMOTION {GAME_MENU_PROMOTION_FOU = 0, GAME_MENU_PROMOTION_TOUR = 1, GAME_MENU_PROMOTION_CAVALIER = 2, GAME_MENU_PROMOTION_REINE = 3};
enum SITUATION_PARTI {ENCOURS = 0, GAGNE = 1, PERDU = 2, NUL = 3};
enum CALCUL_DEPLACEMENT {D_IMPOSSIBLE = 0, D_POSSIBLE = 1, D_PRISE_EN_PASSANT = 2, D_ROQUE = 3, D_MISE_EN_ECHEC = 4, D_ROQUE_CASE_TRAVERSEE = 5};
enum MENU_OUI_NON {MENU_OUI = 0, MENU_NON = 1};

typedef struct PositionItem PositionItem;
struct PositionItem{
    int x, y, width, height;
};

typedef struct GuiMenuItem GuiMenuItem;
struct GuiMenuItem{
    int id;
    SDL_Surface * background;
    SDL_Rect positionBackground;
    PositionItem position;
};

typedef struct MenuPromotion MenuPromotion;
struct MenuPromotion
{
    SDL_Surface * backgroundWhite;
    SDL_Surface * backgroundBlack;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_PROMOTION];
};

typedef struct MenuEchecEtMat MenuEchecEtMat;
struct MenuEchecEtMat
{
    SDL_Surface * backgroundWhite;
    SDL_Surface * backgroundBlack;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_ECHEC_ET_MAT];
};

typedef struct MenuPat MenuPat;
struct MenuPat
{
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_PAT];
};

typedef struct MenuMathNul MenuMatchNul;
struct MenuMathNul
{
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_MATCH_NUL];
};

typedef struct Menu50Coup Menu50Coup;
struct Menu50Coup
{
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_50COUP];
};

typedef struct Menu3Coup Menu3Coup;
struct Menu3Coup
{
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_3COUP];
};

typedef struct Pieces Pieces;
struct Pieces
{
    int id;
    int type;
    int equipe;
    SDL_Surface * background;
    SDL_Rect positionBackground;
    PositionItem position;
    int nombreDeplacement;
    int lastTailleDeplacement;
};

typedef struct Player Player;
struct Player
{
    int win;
};

typedef struct Game Game;
struct Game
{
    Pieces echiquier[WIDTH_ARRAY][WIDTH_ARRAY];
    int echiquierDeplacement[WIDTH_ARRAY][WIDTH_ARRAY]; //permet de savoir quelles sont les cases où l'on peut se déplacer
    Pieces taken[NOMBRE_CASE];
    int nombreTaken; //nombre de piece perdues, utile pour la règle des 50
    string message;
    SDL_Surface * surfaceMessage;
    SDL_Rect positionMessage;
    TTF_Font * policeMessage;
    SDL_Color couleurMessage;
    Player player[2];
    int tourPlayer;
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU_GAME];
    SDL_Surface * surfaceCaseSelected;
    SDL_Surface * surfaceCaseJouable;
    SDL_Surface * surfaceCaseJouableEnnemi;
    int lastDeplacementWhiteX;
    int lastDeplacementWhiteY;
    int lastDeplacementBlackX;
    int lastDeplacementBlackY;
    MenuPromotion menuPromotion;
    MenuEchecEtMat menuEchecEtMat;
    MenuPat menuPat;
    Menu50Coup menu50Coup;
    Menu3Coup menu3Coup;
    MenuMatchNul menuMatchNul;
    int nombreDeplacement; //utiles pour tester la règle des 50
    int nombreDeplacementPossible; //est utilisé pour le calcul d'échec et mat
    bool demandeRegle50Coup; //on ne demande qu'un fois pour les 50 coups
    int nombreCoupLastCapturePromotion; //nombre de coups depuis la dernière prise d'une pièce ou la poussée d'un pion (promotion)
    bool enEchec;
    bool enEchecEtMat;
    bool enPat;
    bool roiRoquePossible;
};

typedef struct Menu Menu;
struct Menu
{
    SDL_Surface * background;
    SDL_Rect positionBackground;
    GuiMenuItem menu[NOMBRE_ITEM_MENU];
};

typedef struct Engine Engine;
struct Engine
{
    int currentAction;
    Game game;
    Menu menu;
    SDL_Surface * window;
};

void initGame(Engine * engine);
void initMenu(Engine * engine);
void initMenuPromotion(Engine * engine);
void initMenuEchecEtMat(Engine * engine);
void initMenuPat(Engine * engine);
void initMenuMatchNul(Engine * engine);
void initMenu50Coup(Engine * engine);
void initMenu3Coup(Engine * engine);
void initPiece(Pieces * piece, int type, int equipe, string sprite, int x, int y);
void initMenuItem(GuiMenuItem * menu, int id, string sprite, int x, int y, int width, int height);
void freeSurface(Engine * engine);
int checkClickMenu(GuiMenuItem * menu, int tailleUtile, PositionItem positionClick);
void writeMessage(Engine * engine);
SDL_Cursor * CreateCursor();
void clearTabDeplacement(Engine * engine);

#endif // FUNCTION_HPP_INCLUDED
