#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include "function.hpp"
#include "game.hpp"

void playTour(Engine * engine);
int getCoordClickX(Engine * engine, PositionItem positionClick);
int getCoordClickY(Engine * engine, PositionItem positionClick);
bool checkIfIsYourPiece(Engine * engine, int clickX, int clickY);
bool checkMoveAvailable(Engine * engine, int clickX, int clickY);
void calculDeplacementPiece(Engine * engine, int clickX, int clickY);
bool checkCaseSelectedDeplacable(Engine * engine, int clickX, int clickY);
int getMonEquipe(Engine * engine);
int getEquipeAdverse(Engine * engine);
void deplacePiece(Engine * engine, int clickX, int clickY, int clickXFinal, int clickYFinal, bool changeBackground = false);
void deplacePion(Engine * engine, int clickX, int clickY, Game * game);
void deplaceCavalier(Engine * engine, int clickX, int clickY, Game * game);
void deplaceFou(Engine * engine, int clickX, int clickY, Game * game);
void deplaceTour(Engine * engine, int clickX, int clickY, Game * game);
void deplaceDame(Engine * engine, int clickX, int clickY, Game * game);
void deplaceRoi(Engine * engine, int clickX, int clickY, Game * game);
void checkPromotion(Engine * engine, int clickXFinal, int clickYFinal);
void checkPriseEnPassant(Engine * engine, int clickX, int clickY);
void checkRoque(Engine * engine, int clickX, int clickY);
bool checkEchec(Engine * engine, int clickX, int clickY);
bool checkEchecEtMat(Engine * engine);
bool checkPat(Engine * engine);
void menuEchecEtMat(Engine * engine);
void menuPat(Engine * engine);
void menuMatchNul(Engine * engine);
bool menu50Coup(Engine * engine);
void menu3Coup(Engine * engine);
void trouverRoi(Engine * engine, PositionItem * position);

#endif // PLAYER_HPP_INCLUDED
