
// drawer.h
#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

int WriteTextCenter(SDL_Renderer *renderer, TTF_Font *font, int cell_size, int i, int j, const char *text);
int WriteTextButton(SDL_Renderer *renderer, TTF_Font *font, SDL_Rect buttonRect, const char *text);
#endif
