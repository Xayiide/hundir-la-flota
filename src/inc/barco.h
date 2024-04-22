#ifndef BARCO_H_
#define BARCO_H_

#include <stdbool.h>  /* bool         */
#include <SDL2/SDL.h> /* SDL_Renderer */

void barco_init();
void barco_imprimir(SDL_Renderer *rnd);
bool barco_celda_ocupada(SDL_Point p);

#endif
