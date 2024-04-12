#include <stdio.h>   /* printf */
#include <stdlib.h>  /* exit, EXIT_SUCCESS */
#include <stdbool.h> /* bool */

#include <SDL2/SDL.h>

#define PXFACTOR   60
#define ANCHO      600
#define ALTO       600
#define PXANCHO    ANCHO / PXFACTOR
#define PXALTO     ALTO  / PXFACTOR
#define NUMCELULAS PXANCHO * PXALTO
#define REFRESH_RT 15 /* ms */

void imprime(bool casillas[PXALTO][PXANCHO], SDL_Renderer *rnd)
{
    int f, c;

    SDL_SetRenderDrawColor(rnd, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rnd);

    SDL_SetRenderDrawColor(rnd, 66, 68, 71, SDL_ALPHA_OPAQUE);
    for (f = 0; f < PXALTO; f++) {
        for (c = 0; c < PXANCHO; c++) {
            if ((c + f) % 2 == 0) {
                SDL_RenderDrawPoint(rnd, f, c);
            }
        }
    }

    return;

}


int main()
{
    SDL_Event     e;
    SDL_Renderer *rnd = NULL;
    SDL_Window   *win = NULL;

    bool run = true;
    bool casillas[PXALTO][PXANCHO];

    printf("Creando ventana de %dx%d píxeles\n", ANCHO, ALTO);
    printf("                   %dx%d casillas\n", PXANCHO, PXALTO);
    printf("Cada casilla es de %dx%d píxeles\n", PXFACTOR, PXFACTOR);

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
        fprintf(stderr, "Error en la inicialización de SDL.\n");
        exit(EXIT_FAILURE);
    }

    win = SDL_CreateWindow("Hundir la flota",
                           SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                           ANCHO, ALTO, SDL_WINDOW_SHOWN);
    if (win == NULL) {
        fprintf(stderr, "Error en la creación de la ventana.\n");
        exit(EXIT_FAILURE);
    }

    rnd = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (rnd == NULL) {
        fprintf(stderr, "Error en la creación del renderer.\n");
        exit(EXIT_FAILURE);
    }

    SDL_RenderSetLogicalSize(rnd, PXANCHO, PXALTO);

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
        }

        imprime(casillas, rnd);

        SDL_RenderPresent(rnd);
        SDL_Delay(REFRESH_RT);
    }

    SDL_DestroyRenderer(rnd);
    SDL_DestroyWindow(win);
    SDL_Quit();

    return EXIT_SUCCESS;
}
