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

void handleGameEvents(SDL_Event* event, bool* gameStarted, GameStatus* currentResponse,
                      int CELL_SIZE, int* currentIndex, int* gameLength,
                      SDL_Rect gameViewport, int GRID_SIZE) {
  if (!(*gameStarted)) return;

  if (event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT) {
    int x = event->button.x - gameViewport.x;
    int y = event->button.y - gameViewport.y;

    // Verifica che il click sia dentro la game box
    if (x >= 0 && x < gameViewport.w && y >= 0 && y < gameViewport.h) {
      int cellX = x / CELL_SIZE;
      int cellY = y / CELL_SIZE;

      // Controllo se sta tentando di "deselezionare" la cella precedente
      if (*currentIndex > 0 &&
          currentResponse[*currentIndex - 1].coordinate_x == cellX &&
          currentResponse[*currentIndex - 1].coordinate_y == cellY) {
        currentResponse[*currentIndex - 1].coordinate_x = -1;
        currentResponse[*currentIndex - 1].coordinate_y = -1;
        currentResponse[*currentIndex - 1].status = false;
        (*currentIndex)--;
      } else {
        // Controlla se è già stata selezionata
        bool alreadySelected = false;
        for (int k = 0; k < *gameLength; k++) {
          if (currentResponse[k].status &&
              currentResponse[k].coordinate_x == cellX &&
              currentResponse[k].coordinate_y == cellY) {
            alreadySelected = true;
            break;
          }
        }

        // Se non è stata selezionata, la aggiunge
        if (!alreadySelected && *currentIndex < *gameLength) {
          currentResponse[*currentIndex].coordinate_x = cellX;
          currentResponse[*currentIndex].coordinate_y = cellY;
          currentResponse[*currentIndex].status = true;
          (*currentIndex)++;
        }
      }
    }
  }
}

void renderGameScreen(SDL_Renderer* renderer, TTF_Font* font, int GRID_SIZE, int CELL_SIZE, int gameLength,
                      GameStatus* initialState, GameStatus* currentResponse, bool* gameStarted, int* currentIndex, int* intialStatesIndex, int offsetX, int offsetY) {
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
        SDL_Rect cell = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1};

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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // nero
        SDL_RenderDrawRect(renderer, &cell);
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
        SDL_Rect cell = {offsetX + j * CELL_SIZE, offsetY + i * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1};
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
            SDL_SetRenderDrawColor(renderer, 220, 0, 0, 255);  // Roosso più acceso per le celle già attivate
          } else {
            SDL_SetRenderDrawColor(renderer, 180, 0, 0, 255);  // Rosso per le celle attivate ma non ancora completate
          }
        } else {
          SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);  // Verde
        }
        SDL_RenderFillRect(renderer, &cell);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // nero
        SDL_RenderDrawRect(renderer, &cell);

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