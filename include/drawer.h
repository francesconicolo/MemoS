
// drawer.h
#ifndef DRAWER_H
#define DRAWER_H

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>

int WriteTextCenter(SDL_Renderer* renderer,TTF_Font *font,int cell_size, int i, int j, const char *text);

#endif
