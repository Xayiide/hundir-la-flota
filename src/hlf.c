#include <stdio.h>   /* printf                          */
#include <stdlib.h>  /* exit, EXIT_SUCCESS, rand, srand */
#include <time.h>    /* time                            */
#include <stdbool.h> /* bool                            */

#include <SDL2/SDL.h>

#define PXFACTOR   60
#define ANCHO      1200
#define ALTO       600
#define PXANCHO    ANCHO / PXFACTOR
#define PXALTO     ALTO  / PXFACTOR
#define NUMCASILLAS PXANCHO * PXALTO
#define REFRESH_RT 15 /* ms */

typedef struct {
    size_t    len; /* Tamaño del barco en píxeles */
    bool      dir; /* Número de píxeles que ocupa */
    SDL_Point pos; /* Coordenadas del barco       */
} barco;

static barco barcos[4];

void barco_imprimir(SDL_Renderer *rnd, barco b)
{
    int i;

    SDL_SetRenderDrawColor(rnd, 86, 232, 105, SDL_ALPHA_OPAQUE);
    if (b.dir == 0) { /* vertical */
        for (i = 0; i < b.len; i++) {
            SDL_RenderDrawPoint(rnd, b.pos.x, b.pos.y + i);
        }
    }
    else { /* horizontal */
        for (i = 0; i < b.len; i++) {
            SDL_RenderDrawPoint(rnd, b.pos.x + i, b.pos.y);
        }
    }
}

void barco_imprimir_barcos(SDL_Renderer *rnd)
{
    int i, j;

    for (i = 0; i < 4; i++) {
        barco_imprimir(rnd, barcos[i]);
    }
}

void barco_crear(int len, int dir, SDL_Point pos)
{
    static int barcos_len = 0;
    barco      b;

    b.len = len;
    b.dir = dir;
    b.pos = pos;

    barcos[barcos_len] = b;
    barcos_len++;
}

/* 2 barcos de 2 */
/* 1 barco de 3 */
/* 1 barco de 4 */
/* TODO: Comprobar que los barcos no se superponen */
void barco_rand() {
    int i;
    int dir;
    int x, y;
    SDL_Point pos;

    srand(time(NULL));
    
    for (i = 0; i < 2; i++) {
        dir = rand() % 2;
        if (dir == 0) { /* vertical */
            x = rand() % (PXANCHO / 2);       /* De 0 a 9 */
            y = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
        }
        else { /* horizontal */
            x = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
            y = rand() % (PXANCHO / 2);       /* De 0 a 9 */
        }
        pos.x = x;
        pos.y = y;
        barco_crear(2, dir, pos);
    }
    /* Repetir una vez para longitud 3 y otra para longitud 4 */
    dir = rand() % 2;
    if (dir == 0) { /* vertical */
        x = rand() % (PXANCHO / 2);       /* De 0 a 9 */
        y = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
    }
    else { /* horizontal */
        x = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
        y = rand() % (PXANCHO / 2);       /* De 0 a 9 */
    }
    pos.x = x;
    pos.y = y;
    barco_crear(3, dir, pos);

    dir = rand() % 2;
    if (dir == 0) { /* vertical */
        x = rand() % (PXANCHO / 2);       /* De 0 a 9 */
        y = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
    }
    else { /* horizontal */
        x = rand() % ((PXANCHO / 2) - 1); /* De 0 a 8 */
        y = rand() % (PXANCHO / 2);       /* De 0 a 9 */
    }
    pos.x = x;
    pos.y = y;
    barco_crear(4, dir, pos);


}

void imprime(bool casillas[PXALTO][PXANCHO], SDL_Renderer *rnd, SDL_Point mouse)
{
    int f, c, i;

    SDL_SetRenderDrawColor(rnd, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rnd);

    /* Están invertidas f y c en la pantalla */
    /* Pintar pantalla de mis barcos */
    SDL_SetRenderDrawColor(rnd, 94, 112, 143, SDL_ALPHA_OPAQUE);
    for (c = 0; c < PXALTO; c++) {
        for (f = 0; f < (PXANCHO / 2); f++) {
            if ((c + f) % 2 == 0)
                SDL_RenderDrawPoint(rnd, f, c);
        }
    }

    /* Pintar pantalla de los barcos enemigos */
    SDL_SetRenderDrawColor(rnd, 61, 44, 44, SDL_ALPHA_OPAQUE);
    for (c = 0; c < PXALTO; c++) {
        for (f = (PXANCHO / 2); f < PXANCHO; f++) {
            if ((c + f) % 2 == 0) {
                SDL_RenderDrawPoint(rnd, f, c);
            }
        }
    }

    /* Pintar mis barcos */
    barco_imprimir_barcos(rnd);

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
    bool          casillas[PXALTO][PXANCHO];

    SDL_Point     mouse;

    printf("Creando ventana de %dx%d píxeles\n", ANCHO, ALTO);
    printf("                   %dx%d casillas (%d)\n", PXANCHO, PXALTO,
                                                       NUMCASILLAS);
    printf("Cada casilla es de %dx%d píxeles\n", PXFACTOR, PXFACTOR);
    printf("PXANCHO: %d\n", PXANCHO);
    printf("PXALTO:  %d\n", PXALTO);

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

    barco_rand();

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
            else if (e.type = SDL_MOUSEMOTION) {
                int auxx, auxy, x, y;
                SDL_GetMouseState(&auxx, &auxy);
                x = (auxx / (int) PXFACTOR);
                y = (auxy / (int) PXFACTOR);

                mouse.x = x;
                mouse.y = y;
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
