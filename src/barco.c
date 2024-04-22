#include <stdlib.h>    /* size_t, rand, srand */
#include <time.h>      /* time                */
#include <stdio.h>     /* NULL                */
#include <stdbool.h>   /* true, false         */

#include <SDL2/SDL.h>  /* SDL_Point           */

#include "inc/hlf.h"
#include "inc/barco.h"


#define BARCO_NUM_BARCOS 4


typedef enum {
    VERTICAL   = 0,
    HORIZONTAL = 1
} barco_dir_e;

typedef enum {
    ALIADO  = 0,
    ENEMIGO = 1
} barco_bando_e;

typedef struct {
    size_t        len;
    barco_dir_e   dir;
    barco_bando_e bando;
    SDL_Point     pos;
} barco_t;

typedef enum {
    AGUA   = 0,
    BARCO  = 1,
    TOCADO = 2,
} barco_celda_e;

//static barco_t barcos_aliados[BARCO_NUM_BARCOS];
//static barco_t enemy_ships[BARCO_NUM_BARCOS];

static barco_celda_e barcos_aliados[FILAS][COLUMNAS];
static barco_celda_e barcos_enemigos[FILAS][COLUMNAS];


static bool        barco_crear   (size_t len, barco_bando_e bando);
static barco_dir_e barco_rand_dir();
static void        barco_rand_pos(barco_t *b);
static bool        barco_validar (barco_t b);
static void        barco_guardar (barco_t b);


bool barco_crear(size_t len, barco_bando_e bando)
{
    bool        ret = false;
    int         num_intentos = 0;
    barco_t     barco;

    barco.len   = len;
    barco.bando = bando;

    do {
        barco.dir = barco_rand_dir();
        barco_rand_pos(&barco);
        ret = barco_validar(barco);
        num_intentos++;
    } while ((ret == false) && (num_intentos < 5));

    if (ret == false) {
        fprintf(stderr, "error creando barco:\n");
        fprintf(stderr, "    dir: %d\n",  barco.dir);
        fprintf(stderr, "    len: %ld\n", barco.len);
        fprintf(stderr, "    x:   %d\n",  barco.pos.x);
        fprintf(stderr, "    y:   %d\n",  barco.pos.y);
    }
    else {
        //barco_guardar(dir, len, c, f);
        barco_guardar(barco);
        printf("Barco guardado:\n");
        printf("    dir: %d\n",  barco.dir);
        printf("    len: %ld\n", barco.len);
        printf("    x:   %d\n",  barco.pos.x);
        printf("    y:   %d\n",  barco.pos.y);
    }

    return ret;
}

barco_dir_e barco_rand_dir()
{
    barco_dir_e ret;

    ret = (barco_dir_e) (rand() % 2);

    return ret;
}

void barco_rand_pos(barco_t *b)
{
    if (b->dir == VERTICAL) {
        b->pos.x = (int) (rand() % (COLUMNAS));
        b->pos.y = (int) (rand() % (FILAS - b->len));
    }
    else if (b->dir == HORIZONTAL) {
        b->pos.x = (int) (rand() % (COLUMNAS - b->len));
        b->pos.y = (int) (rand() % (FILAS));
    }

    return;
}

bool barco_validar(barco_t b)
{
    size_t        i, f, c;
    bool          ret = true;
    barco_celda_e barcos[FILAS][COLUMNAS];

    /* TODO: Estaría bien no tener que hacer esto sino utilizar un puntero
     * o algo directamente */
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            if (b.bando == ALIADO) {
                barcos[f][c] = barcos_aliados[f][c];
            }
            else if (b.bando == ENEMIGO) {
                barcos[f][c] = barcos_enemigos[f][c];
            }
        }
    }


    /* TODO: no se permiten barcos adyacentes */
    if (b.dir == VERTICAL) {
        for (i = 0; i < b.len; i++) {
            if (((i + b.pos.y) < FILAS)
                    && (barcos[b.pos.y + i][b.pos.x] == AGUA)) {
                continue;
            }
            else {
                ret = false;
                break;
            }
        }
    }
    else if (b.dir == HORIZONTAL) {
        for (i = 0; i < b.len; i++) {
            if (((i + b.pos.x) < COLUMNAS)
                    && (barcos[b.pos.y][b.pos.x + i] == AGUA)) {
                continue;
            }
            else {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

void barco_guardar(barco_t b)
{
    size_t i;

    if (b.dir == VERTICAL) {
        for (i = 0; i < b.len; i++) {
            if (b.bando == ALIADO)
                barcos_aliados[b.pos.y + i][b.pos.x] = BARCO;
            else if (b.bando == ENEMIGO)
                barcos_enemigos[b.pos.y + i][b.pos.x] = BARCO;
        }
    }
    else if (b.dir == HORIZONTAL) {
        for (i = 0; i < b.len; i++) {
            if (b.bando == ALIADO)
                barcos_aliados[b.pos.y][b.pos.x + i] = BARCO;
            else if (b.bando == ENEMIGO)
                barcos_enemigos[b.pos.y][b.pos.x + i] = BARCO;
        }
    }

    return;
}

void barco_init()
{
    bool ret;
    int  f, c;
    srand((unsigned int) time(NULL));

    /* Inicializa matriz de barcos */
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            barcos_aliados[f][c] = AGUA;
        }
    }

    /* 2 barcos de 2, 1 de 3 y 1 de 4 */
    ret  = barco_crear((size_t) 2, ALIADO);
    ret |= barco_crear((size_t) 2, ALIADO);
    ret |= barco_crear((size_t) 3, ALIADO);
    ret |= barco_crear((size_t) 4, ALIADO);

    ret |= barco_crear((size_t) 2, ENEMIGO);
    ret |= barco_crear((size_t) 2, ENEMIGO);
    ret |= barco_crear((size_t) 3, ENEMIGO);
    ret |= barco_crear((size_t) 4, ENEMIGO);

    if (ret == false) {
        fprintf(stderr, "No se han podido inicializar los barcos.\n");
        exit(EXIT_FAILURE);
    }

    return;
}

void barco_imprimir(SDL_Renderer *rnd)
{
    int f, c;

    /* Imprime barcos aliados */
    SDL_SetRenderDrawColor(rnd, 86, 232, 105, SDL_ALPHA_OPAQUE);
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            if (barcos_aliados[f][c] == BARCO) {
                SDL_RenderDrawPoint(rnd, c, f);
            }
        }
    }

    /* Imprime barcos enemigos */
    SDL_SetRenderDrawColor(rnd, 168, 98, 50, SDL_ALPHA_OPAQUE);
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            if (barcos_enemigos[f][c] == BARCO) {
                SDL_RenderDrawPoint(rnd, c + COLUMNAS, f);
            }
        }
    }

    return;
}

