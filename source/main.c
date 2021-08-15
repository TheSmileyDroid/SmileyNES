#include <stdio.h>
#include "Bus.h"
#include "Smiley6502.h"
#define SDL_MAIN_HANDLED
#include "../include/SDL2/SDL.h"
#include "../include/SDL2/SDL_ttf.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

/*
void drawRAM(sfRenderWindow *window, int x, int y, uint16_t nAddr, int nRows, int nColumns, sfFont *font)
{
    
}
*/
int main()
{

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    TTF_Init();

    TTF_Font *font = TTF_OpenFont("roboto.ttf", 14);
    SDL_Color White = {255, 255, 255};

    //The surface contained by the window
    SDL_Surface *screenSurface = NULL;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    window = SDL_CreateWindow("SmileyNES", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    renderer = SDL_CreateRenderer(window, -1, 0);

    //Main loop flag
    bool quit = false;

    //Event handler
    SDL_Event e;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);
        SDL_RenderClear(renderer);

        SDL_Surface *surfaceMessage =
            TTF_RenderText_Solid(font, "put your text here", White);

        SDL_Texture *Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

        SDL_Rect Message_rect;              //create a rect
        Message_rect.x = 100;               //controls the rect's x coordinate
        Message_rect.y = 100;               // controls the rect's y coordinte
        Message_rect.w = surfaceMessage->w; // controls the width of the rect
        Message_rect.h = surfaceMessage->h; // controls the height of the rect

        SDL_RenderCopy(renderer, Message, NULL, &Message_rect);

        SDL_RenderPresent(renderer);

        SDL_FreeSurface(surfaceMessage);
        SDL_DestroyTexture(Message);
    }

    return 0;
}