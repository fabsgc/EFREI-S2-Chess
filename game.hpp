#ifndef GAME_HPP_INCLUDED
#define GAME_HPP_INCLUDED

#include "function.hpp"

void drawGame(Engine * engine);
void drawPieces(Engine * engine);
void drawPiecesDead(Engine * engine);
void drawPieceJouable(Engine * engine,  int clickX, int clickY);
void drawMenuPromotion(Engine * engine);
void drawMenuEchecEtMat(Engine * engine);
void drawMenuPat(Engine * engine);
void drawMenuMatchNul(Engine * engine);
void drawMenu50Coup(Engine * engine);
void drawMenu3Coup(Engine * engine);
void playGame(Engine * engine);

#endif // GAME_HPP_INCLUDED
