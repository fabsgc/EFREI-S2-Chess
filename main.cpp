#include <iostream>
#include "function.hpp"
#include "engine.hpp"

using namespace std;

int main(int argc, char *argv[]){
    Engine engine;

    initGame(&engine);
    initMenu(&engine);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_WM_SetIcon(SDL_LoadBMP("sprites/icon.bmp"), NULL);
    engine.window = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE | SDL_DOUBLEBUF);
    SDL_WM_SetCaption("Chess Game By EFREI", NULL);
    SDL_SetCursor(CreateCursor());

    while (engine.currentAction != QUIT){
        run(&engine);
        SDL_Flip(engine.window);
        SDL_FillRect(engine.window, NULL, SDL_MapRGB(engine.window->format, 255, 255, 255));
    }

    freeSurface(&engine);
    SDL_Quit();

    return EXIT_SUCCESS; // Fermeture du programme
}
