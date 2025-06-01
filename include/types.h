
// types.h
#ifndef TYPES_H
#define TYPES_H

typedef enum {
  SETUP_SCREEN,
  GAME_SCREEN,
  END_SCREEN
} AppState;

typedef struct {
  SDL_Rect area;  // bounding box per click
  char* label;    // testo accanto
  int selected;   // 1 = selezionato
} RadioButton;
typedef struct
{
  int coordinate_x;
  int coordinate_y;
  bool status;
} GameStatus;
#endif
