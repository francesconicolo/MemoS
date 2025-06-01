
// menuScreen.h
#ifndef MENUSCREEN_H
#define MENUSCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "types.h"
void renderMenuScreen(SDL_Renderer* renderer, TTF_Font* font, int gameLength);
void handleMenuEvents(SDL_Event* event, AppState* currentState, int* gameLength, int* delay,
                      GameStatus** initialState, GameStatus** currentResponse, int GRID_SIZE);
#endif
