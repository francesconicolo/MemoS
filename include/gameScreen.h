
// gamescreen.h
#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <time.h>

#include "types.h"
void initGame(GameStatus** gameStatus, int GRID_SIZE, int gameLength);
void initResponse(GameStatus** gameStatus, int GRID_SIZE, int gameLength);
void renderGameScreen(SDL_Renderer* renderer, TTF_Font* font, int GRID_SIZE, int CELL_SIZE, int gameLength, GameStatus* initialState, GameStatus* currentState, bool* gameStarted, int* currentIndex, int* intialStatesIndex, int offsetX, int offsetY, int delay);
void handleGameEvents(SDL_Event* event, bool* gameStarted, GameStatus* currentResponse, int CELL_SIZE, int* currentIndex, int* gameLength, SDL_Rect gameViewport, int GRID_SIZE);
#endif
