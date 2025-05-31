#include "drawer.h"

int WriteTextCenter(SDL_Renderer *renderer, TTF_Font *font, int cell_size, int i, int j, const char *text) {
  SDL_Color textColor = {0, 0, 0, 255};  // nero
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, textColor);
  if (!textSurface) return 1;

  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
  if (!textTexture) {
    SDL_FreeSurface(textSurface);
    return 1;
  }
  SDL_Rect textRect;
  textRect.x = j * cell_size + (cell_size - textSurface->w) / 2;
  textRect.y = i * cell_size + (cell_size - textSurface->h) / 2;
  textRect.w = textSurface->w;
  textRect.h = textSurface->h;

  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
  return 0;
}
