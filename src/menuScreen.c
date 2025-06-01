#include "menuScreen.h"

#include "drawer.h"
#include "gameScreen.h"

#define WINDOW_SIZE_W 1000
#define WINDOW_SIZE_H 700
#define NUM_BUTTONS_DELAY 9
#define NUM_BUTTONS_LENGTH_GAME 9
RadioButton radioButtonsDelay[NUM_BUTTONS_DELAY];
RadioButton radioButtonsLengthGame[NUM_BUTTONS_LENGTH_GAME];
int selectedButtonIndexDelay = 0;
int selectedButtonIndexLengthGame = 0;

void handleMenuEvents(SDL_Event* event, AppState* currentState,
                      int* gameLength, int* delay, GameStatus** initialState,
                      GameStatus** currentResponse, int GRID_SIZE) {
  if (event->type == SDL_MOUSEBUTTONDOWN) {
    int x = event->button.x;
    int y = event->button.y;
    for (int i = 0; i < NUM_BUTTONS_DELAY; i++) {
      SDL_Rect area = radioButtonsDelay[i].area;
      if (x >= area.x && x <= area.x + area.w &&
          y >= area.y && y <= area.y + area.h) {
        selectedButtonIndexDelay = i;
        (*delay) = (i + 1) * 100;  // Imposta il delay in base al pulsante selezionato
        for (int j = 0; j < NUM_BUTTONS_DELAY; j++) {
          radioButtonsDelay[j].selected = (j == i);
        }
      }
    }
    for (int i = 0; i < NUM_BUTTONS_LENGTH_GAME; i++) {
      SDL_Rect area = radioButtonsLengthGame[i].area;
      if (x >= area.x && x <= area.x + area.w &&
          y >= area.y && y <= area.y + area.h) {
        selectedButtonIndexLengthGame = i;
        (*gameLength) = i + 4;  // Imposta la lunghezza del gioco in base al pulsante selezionato
        for (int j = 0; j < NUM_BUTTONS_LENGTH_GAME; j++) {
          radioButtonsLengthGame[j].selected = (j == i);
        }
      }
    }
    // Verifica se il click è dentro il rettangolo del pulsante "Play"
    SDL_Rect playButton = {(WINDOW_SIZE_W / 2) - 200 / 2, WINDOW_SIZE_H - 100, 200, 50};
    if (x >= playButton.x && x <= playButton.x + playButton.w &&
        y >= playButton.y && y <= playButton.y + playButton.h) {
      initGame(initialState, GRID_SIZE, (*gameLength));         // Inizializza lo stato del gioco con nuove coordinate
      initResponse(currentResponse, GRID_SIZE, (*gameLength));  // Inizializza lo stato corrente con nuove coordinate
      *currentState = 1;                                        // Cambia stato a GAME_SCREEN
    }
  }
}

void initRadioButtonsDelay() {
  int startX = 100;
  int startY = 170;
  int spacing = 40;

  for (int i = 0; i < NUM_BUTTONS_DELAY; i++) {
    radioButtonsDelay[i].area.x = startX + i * spacing;
    radioButtonsDelay[i].area.y = startY;
    radioButtonsDelay[i].area.w = 20;
    radioButtonsDelay[i].area.h = 20;
    radioButtonsDelay[i].label = malloc(10 * sizeof(char));
    if (radioButtonsDelay[i].label != NULL) {
      snprintf(radioButtonsDelay[i].label, 10, "  %d  ", (i + 1));
    }
    radioButtonsDelay[i].selected = (i == selectedButtonIndexDelay);
  }
}

void initRadioButtonsLengthGame() {
  int startX = 100;
  int startY = 300;
  int spacing = 40;

  for (int i = 0; i < NUM_BUTTONS_LENGTH_GAME; i++) {
    radioButtonsLengthGame[i].area.x = startX + i * spacing;
    radioButtonsLengthGame[i].area.y = startY;
    radioButtonsLengthGame[i].area.w = 20;
    radioButtonsLengthGame[i].area.h = 20;
    radioButtonsLengthGame[i].label = malloc(10 * sizeof(char));
    if (radioButtonsLengthGame[i].label != NULL) {
      snprintf(radioButtonsLengthGame[i].label, 10, "  %d  ", (i + 4));
    }
    radioButtonsLengthGame[i].selected = (i == selectedButtonIndexLengthGame);
  }
}

void renderRadioButton(SDL_Renderer* renderer, TTF_Font* font, RadioButton* button) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
  SDL_RenderDrawRect(renderer, &button->area);

  // Se selezionato → riempi interno
  if (button->selected) {
    SDL_Rect inner = {
        button->area.x + 5,
        button->area.y + 5,
        button->area.w - 10,
        button->area.h - 10};
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &inner);
  }

  // Testo accanto
  // Aggiungi uno spacing orizzontale tra i label (ad esempio 10 pixel)

  SDL_Rect labelRect = {
      button->area.x + button->area.w / 2,
      button->area.y - 20,
      0, 0};
  WriteTextButton(renderer, font, labelRect, button->label);
}

void renderMenuScreen(SDL_Renderer* renderer, TTF_Font* font, int gameLength) {
  initRadioButtonsDelay();
  initRadioButtonsLengthGame();
  SDL_Rect delayLabel = {120, 100, 13, 20};
  WriteTextButton(renderer, font, delayLabel, "Delay");
  for (int i = 0; i < NUM_BUTTONS_DELAY; i++) {
    renderRadioButton(renderer, font, &radioButtonsDelay[i]);
  }
  SDL_Rect gameLengthLabel = {120, 230, 95, 20};
  WriteTextButton(renderer, font, gameLengthLabel, "Durata gioco");
  for (int i = 0; i < NUM_BUTTONS_LENGTH_GAME; i++) {
    renderRadioButton(renderer, font, &radioButtonsLengthGame[i]);
  }

  SDL_Rect PlayButton = {(WINDOW_SIZE_W / 2) - 200 / 2, WINDOW_SIZE_H - 100, 200, 50};
  SDL_SetRenderDrawColor(renderer, 0, 128, 255, 255);  // blu
  SDL_RenderFillRect(renderer, &PlayButton);
  WriteTextButton(renderer, font, PlayButton, "Play");
}