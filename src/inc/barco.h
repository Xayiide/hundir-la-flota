#ifndef BARCO_H_
#define BARCO_H_

#include <stdbool.h>  /* bool         */
#include <SDL2/SDL.h> /* SDL_Renderer */

void barco_init();
void barco_imprimir(SDL_Renderer *rnd);
bool barco_crear   (size_t len, barco_bando_e bando);

/* Esta función sólo se llama desde partida.c. Podría estar en una interfaz
 * entre partida y barco para que hlf.c no la vea */
bool barco_dispara_celda(SDL_Point p);

#endif
