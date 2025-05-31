#include "gameScreen.h"

#include "drawer.h"
void initGame(GameStatus* gameStatus, int GRID_SIZE, int gameLength) {
  srand(time(0));

  // 1. Crea tutte le coordinate possibili
  int totalCells = GRID_SIZE * GRID_SIZE;
  int coords[totalCells][2];
  int index = 0;
  for (int y = 0; y < GRID_SIZE; y++) {
    for (int x = 0; x < GRID_SIZE; x++) {
      coords[index][0] = x;
      coords[index][1] = y;
      index++;
    }
  }

  // 2. Fisher-Yates shuffle
  for (int i = totalCells - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int tmpX = coords[i][0];
    int tmpY = coords[i][1];
    coords[i][0] = coords[j][0];
    coords[i][1] = coords[j][1];
    coords[j][0] = tmpX;
    coords[j][1] = tmpY;
  }

  // 3. Prendi le prime gameLength coordinate
  for (int i = 0; i < gameLength; i++) {
    gameStatus[i].coordinate_x = coords[i][0];
    gameStatus[i].coordinate_y = coords[i][1];
    gameStatus[i].status = false;
  }
}

void initResponse(GameStatus* gameStatus, int GRID_SIZE, int gameLength) {
  // Inizializza lo stato del gioco con coordinate casuali
  for (int i = 0; i < gameLength; i++) {
    gameStatus[i].coordinate_x = -1;
    gameStatus[i].coordinate_y = -1;
    gameStatus[i].status = false;  // Inizializza lo stato a false
  }
}

void handleGameEvents(SDL_Event* event, bool* gameStarted, GameStatus* currentResponse, int CELL_SIZE, int* currentIndex, int* gameLength) {
  if ((*gameStarted) == true) {
    if (event->type == SDL_MOUSEBUTTONDOWN && (*currentIndex) <= (*gameLength)) {
      int x = event->button.x / CELL_SIZE;
      int y = event->button.y / CELL_SIZE;
      if (currentResponse[(*currentIndex) - 1].coordinate_x == x &&
          currentResponse[(*currentIndex) - 1].coordinate_y == y) {
        currentResponse[(*currentIndex) - 1].coordinate_x = -1;
        currentResponse[(*currentIndex) - 1].coordinate_y = -1;
        currentResponse[(*currentIndex) - 1].status = false;  // Disattiva la cella corrente
        (*currentIndex)--;                                    // Torna indietro di un passo
      } else {
        bool found = false;
        for (int k = 0; k < (*gameLength); k++) {
          if (currentResponse[k].status == true &&
              currentResponse[k].coordinate_x == x &&
              currentResponse[k].coordinate_y == y) {
            found = true;
          }
        }
        if (found == false && (*currentIndex) < (*gameLength)) {
          currentResponse[(*currentIndex)].coordinate_x = x;  // Imposta la coordinata x
          currentResponse[(*currentIndex)].coordinate_y = y;  // Imposta la coordinata y
          currentResponse[(*currentIndex)].status = true;     // Attiva la cella corrente
          (*currentIndex)++;
        }
      }
    }
  }
}

void renderGameScreen(SDL_Renderer* renderer, TTF_Font* font, int GRID_SIZE, int CELL_SIZE, int gameLength,
                      GameStatus* initialState, GameStatus* currentResponse, bool* gameStarted, int* currentIndex, int* intialStatesIndex) {
  static Uint32 lastTick = 0;  // Persistente tra chiamate
  if ((*gameStarted) == false) {
    Uint32 now = SDL_GetTicks();
    if (now - lastTick >= 500) {
      if ((*intialStatesIndex) >= gameLength) {
        *gameStarted = true;  // Inizia il gioco
      } else {
        initialState[(*intialStatesIndex)].status = true;  // Attiva la cella corrente
        initialState[(*intialStatesIndex) - 1].status = false;
        (*intialStatesIndex)++;  // Passa alla prossima cella
        lastTick = now;
      }
    }

    for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
        SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2};

        // Controlla se in questa cella c'è qualcosa da scrivere
        int index = -1;
        for (int k = 0; k < gameLength; k++) {
          if (initialState[k].coordinate_x == j && initialState[k].coordinate_y == i && initialState[k].status == true) {
            index = k;
            break;
          }
        }

        if (index != -1) {
          SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  // Rosso
        } else {
          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);  // Verde
        }

        SDL_RenderFillRect(renderer, &cell);

        if (index != -1) {
          int num_digits = snprintf(NULL, 0, "%d", index) + 1;
          char* buffer = (char*)malloc(num_digits);
          if (buffer == NULL) {
            printf("Errore di allocazione memoria\n");
            return;
          }
          sprintf(buffer, "%d", index);
          if (WriteTextCenter(renderer, font, CELL_SIZE, i, j, buffer) == 1) {
            printf("Errore nel disegnare il testo\n");
          }
          free(buffer);
        }
      }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);  // ~60 FPS
  } else {
    for (int i = 0; i < GRID_SIZE; i++) {
      for (int j = 0; j < GRID_SIZE; j++) {
        SDL_Rect cell = {j * CELL_SIZE, i * CELL_SIZE, CELL_SIZE - 2, CELL_SIZE - 2};
        int index = -1;
        for (int k = 0; k < gameLength; k++) {
          if (currentResponse[k].coordinate_x == j &&
              currentResponse[k].coordinate_y == i &&
              currentResponse[k].status == true) {
            index = k;
            break;
          }
        }
        if (index != -1) {
          if (index == *currentIndex - 1) {
            SDL_SetRenderDrawColor(renderer, 220, 0, 0, 255);  // Blu per le celle già attivate
          } else {
            SDL_SetRenderDrawColor(renderer, 180, 0, 0, 255);  // Rosso per le celle attivate ma non ancora completate
          }
        } else {
          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);  // Verde
        }
        SDL_RenderFillRect(renderer, &cell);
        if (index != -1) {
          int num_digits = snprintf(NULL, 0, "%d", index) + 1;
          char* buffer = (char*)malloc(num_digits);
          if (buffer == NULL) {
            printf("Errore di allocazione memoria\n");
            return;
          }
          sprintf(buffer, "%d", index);
          if (WriteTextCenter(renderer, font, CELL_SIZE, i, j, buffer) == 1) {
            printf("Errore nel disegnare il testo\n");
          }
          free(buffer);
        }
      }
    }
    SDL_RenderPresent(renderer);
    SDL_Delay(16);  // ~60 FPS
  }
}