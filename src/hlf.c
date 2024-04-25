#include <stdio.h>   /* printf                          */
#include <stdlib.h>  /* exit, EXIT_SUCCESS, rand, srand */
#include <time.h>    /* time                            */
#include <stdbool.h> /* bool                            */

#include <SDL2/SDL.h>

#include "inc/barco.h"
#include "inc/hlf.h"

#define PXFACTOR    60
#define PXCOLUMNAS  (COLUMNAS * 2) * PXFACTOR /* 20 * 60 = 1200 px */
#define PXFILAS     FILAS  * PXFACTOR /* 20 * 60 = 600 px */
#define REFRESH_RT  15 /* ms */

void imprime(bool casillas[FILAS][COLUMNAS * 2], SDL_Renderer *rnd, SDL_Point mouse)
{
    int f, c;

    SDL_SetRenderDrawColor(rnd, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rnd);

    /* Están invertidas f y c en la pantalla */
    /* Pintar pantalla de mis barcos */
    SDL_SetRenderDrawColor(rnd, 94, 112, 143, SDL_ALPHA_OPAQUE);
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            if ((c + f) % 2 == 0) {
                SDL_RenderDrawPoint(rnd, c, f);
            }
        }
    }

    /* Pintar pantalla de los barcos enemigos */
    SDL_SetRenderDrawColor(rnd, 61, 44, 44, SDL_ALPHA_OPAQUE);
    for (f = 0; f < FILAS; f++) {
        for (c = COLUMNAS; c < (COLUMNAS * 2); c++) {
            if ((c + f) % 2 == 0) {
                SDL_RenderDrawPoint(rnd, c, f);
            }
        }
    }

    /* Pintar mis barcos */
    barco_imprimir(rnd);

    /* Pintar el ratón */
    SDL_SetRenderDrawColor(rnd, 0, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderDrawPoint(rnd, mouse.x, mouse.y);

    return;
}


int main()
{
    SDL_Event     e;
    SDL_Renderer *rnd = NULL;
    SDL_Window   *win = NULL;

    bool          run = true;
    bool          casillas[FILAS][COLUMNAS * 2];

    SDL_Point     mouse;

    printf("Creando ventana de %dx%d píxeles\n", PXCOLUMNAS, PXFILAS);
    printf("                   %dx%d casillas (%d)\n", COLUMNAS * 2, FILAS,
                                                       NUMCASILLAS);
    printf("Cada casilla es de %dx%d píxeles\n", PXFACTOR, PXFACTOR);
    printf("PXCOLUMNAS: %d\n", PXCOLUMNAS);
    printf("PXFILAS:  %d\n", PXFILAS);

    barco_init();

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "Error en la inicialización de SDL.\n");
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow("Hundir la flota",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           PXCOLUMNAS, PXFILAS, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "Error en la creación de la ventana.\n");
        exit(EXIT_FAILURE);
    }

    rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL) {
        fprintf(stderr, "Error en la creación del renderer.\n");
        exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(rnd, COLUMNAS * 2, FILAS);


    while (run) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                run = false;
            }
            else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_q:
                        run = false;
                        break;
                }
            }
            else if (e.type == SDL_MOUSEMOTION) {
                int auxx, auxy, x, y;
                SDL_GetMouseState(&auxx, &auxy);
                x = (auxx / (int) PXFACTOR);
                y = (auxy / (int) PXFACTOR);

                mouse.x = x;
                mouse.y = y;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int auxx, auxy;
                SDL_Point p;
                SDL_GetMouseState(&auxx, &auxy);
                p.x = (auxx / (int) PXFACTOR);
                p.y = (auxy / (int) PXFACTOR);

                if (p.x >= COLUMNAS) {
                    printf("Comprobando (x, y) (%d, %d)\n", p.x, p.y);
                    if (barco_celda_ocupada(p) == true) {
                        printf("Tocado! (x, y) (%d, %d)\n", p.x, p.y);
                    }
                }


            }
        }

        imprime(casillas, rnd, mouse);

        SDL_RenderPresent(rnd);
        SDL_Delay(REFRESH_RT);
    }

    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
