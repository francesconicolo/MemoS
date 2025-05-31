#include <SDL2/SDL.h>
#include <drawer.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "gameScreen.h"
#define WINDOW_SIZE 500
#define GRID_SIZE 5
#define CELL_SIZE (WINDOW_SIZE / GRID_SIZE)
#define NUM_RADIO 3

typedef enum {
  SETUP_SCREEN,
  GAME_SCREEN,
  END_SCREEN
} AppState;

AppState currentState = GAME_SCREEN;

int main() {
  SDL_Init(SDL_INIT_VIDEO);
  TTF_Init();

  TTF_Font *font = TTF_OpenFont("font/Roboto.ttf", 24);
  if (!font) {
    printf("Errore font: %s\n", TTF_GetError());
    exit(1);
  }
  SDL_Window *window = SDL_CreateWindow(
      "Memos",
      SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      WINDOW_SIZE, WINDOW_SIZE, 0);

  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_RendererInfo info;
  SDL_GetRendererInfo(renderer, &info);
  printf("Renderer in uso: %s\n", info.name);
  if (!window) {
    printf("Errore creazione finestra: %s\n", SDL_GetError());
    return 1;
  }
  if (!renderer) {
    printf("Errore creazione renderer: %s\n", SDL_GetError());
    return 1;
  }

  bool gameStarted = false;  // Inizializza lo stato del gioco
  int gameLength = 10;       // Numero di celle da attivare
  GameStatus *initialState = malloc(gameLength * sizeof(GameStatus));
  GameStatus *currentResponse = malloc(gameLength * sizeof(GameStatus));
  if (!initialState || !currentResponse) {
    printf("Errore malloc\n");
    return 1;
  }
  initGame(initialState, GRID_SIZE, gameLength);         // Inizializza lo stato del gioco con nuove coordinate
  initResponse(currentResponse, GRID_SIZE, gameLength);  // Inizializza lo stato corrente con nuove coordinate
  int currentIndex = 0;                                  // Indice corrente per il ciclo di attivazione
  int intialStatesIndex = 0;
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_QUIT) {
        running = false;
      }

      switch (currentState) {
        case SETUP_SCREEN:
          printf("Handling setup screen events\n");
          // handleSetupEvents(&event);
          break;
        case GAME_SCREEN:
          handleGameEvents(&event, &gameStarted, currentResponse, CELL_SIZE, &currentIndex, &gameLength);
          break;
        case END_SCREEN:
          printf("Handling end screen events\n");
          // handleGameEndEvents(&event);
          break;
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    switch (currentState) {
      case SETUP_SCREEN:
        // renderSetupScreen(renderer, font);
        printf("Rendering setup screen\n");
        break;
      case GAME_SCREEN:
        renderGameScreen(renderer, font, GRID_SIZE, CELL_SIZE, gameLength, initialState, currentResponse, &gameStarted, &currentIndex, &intialStatesIndex);
        break;
      case END_SCREEN:
        printf("Rendering end screen\n");
        // renderEndScreen(renderer, font);
        break;
    }

    SDL_RenderPresent(renderer);  // <-- ti mancava anche questa bro!
  }

  // Cleanup dopo il ciclo
  free(initialState);
  free(currentResponse);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
  return 0;
}