#ifndef PARTIDA_H_
#define PARTIDA_H_

#include <stdint.h>   /* uint */
#include <stdbool.h>  /* bool */
#include <SDL2/SDL.h>

void partida_init(uint8_t filas, uint8_t cols);
void partida_disparo_aliado(SDL_Point p);
void partida_disparo_enemigo();
bool partida_esta_acabada();

#endif
