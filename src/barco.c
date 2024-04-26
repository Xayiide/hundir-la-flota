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

static barco_celda_e barcos_aliados[FILAS][COLUMNAS];
static barco_celda_e barcos_enemigos[FILAS][COLUMNAS];


static barco_dir_e barco_rand_dir();
static void        barco_rand_pos(barco_t *b);
static bool        barco_validar (barco_t b);
static void        barco_guardar (barco_t b);
static bool        barco_hay_adyacentes(barco_bando_e bando, size_t f, size_t c);
static bool        barco_se_sale(int f, int c);


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
    } while (ret == false);

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
    /* Copia un array de barcos para no tener que comprobar más adelante si son
     * aliados o enemigos */
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
            /* Si la posición es válida y la celda es AGUA */
            if (((b.pos.y + i) < FILAS) &&
                (barcos[b.pos.y + i][b.pos.x] == AGUA)) {
                /* Comprobamos sus adyacentes */
                if (barco_hay_adyacentes(b.bando, b.pos.y + i, b.pos.x) == true) {
                    ret = false;
                    break;
                }
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
            if (((b.pos.x + i) < COLUMNAS) &&
                (barcos[b.pos.y][b.pos.x + i] == AGUA)) {
                if (barco_hay_adyacentes(b.bando, b.pos.y, b.pos.x + i) == true) {
                    ret = false;
                    break;
                }
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

bool barco_hay_adyacentes(barco_bando_e bando, size_t f, size_t c) {
    bool ret = false;
    int  i, fila, col;

    int despl[4][2] = {
                 {-1,  0},
        {0, -1},           {0,  1},
                 { 1,  0}
    };


    if (barco_se_sale((int) f, (int) c) == true) {
        fprintf(stderr, "Fuera de rango\n");
        ret = false;
        return ret;
    }


    for (i = 0; i < 4; i++) {
        fila = (int) f + despl[i][0];
        col  = (int) c + despl[i][1];
        if (barco_se_sale(fila, col) == false) {
            if ((bando == ALIADO) && (barcos_aliados[fila][col] != AGUA)) {
                ret = true;
                break;
            }
            else if ((bando == ENEMIGO) && (barcos_enemigos[fila][col] != AGUA)) {
                ret = true;
                break;
            }
        }
    }

    return ret;
}

bool barco_se_sale(int f, int c) {
    bool ret = false;

    if ((f >= FILAS) || (c >= COLUMNAS))
        ret = true;
    else if ((f < 0) || (c < 0))
        ret = true;

    return ret;
}


void barco_init()
{
    bool ret;
    int  f, c;
    srand((unsigned int) time(NULL));

    /* Inicializa matriz de barcos */
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            barcos_aliados[f][c]  = AGUA;
            barcos_enemigos[f][c] = AGUA;
        }
    }

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

bool barco_crear_b(size_t len, barco_dir_e dir, int f, int c)
{
    barco_t b;
    bool    ret;

    b.bando = ALIADO;
    b.len   = len;
    b.dir   = dir;
    b.pos.x = c;
    b.pos.y = f;

    ret = barco_validar(b);
    if (ret == true) {
        barco_guardar(b);
    }

    return ret;
}

bool barco_dispara_celda(SDL_Point p)
{
    bool ret = false;


    /* Disparo enemigo */
    if (p.x >= COLUMNAS) {
        printf("Comprobando barcos_enemigos[%d][%d]\n", p.y, p.x - COLUMNAS);
        if (barcos_enemigos[p.y][p.x - COLUMNAS] == BARCO) {
            barcos_enemigos[p.y][p.x - COLUMNAS] = TOCADO;
            ret = true;
        }
    }
    /* Disparo aliado */
    else if (p.x < COLUMNAS) {
        printf("Comprobando barcos_aliados[%d][%d]\n", p.y, p.x);
        if (barcos_aliados[p.y][p.x] == BARCO) {
            barcos_aliados[p.y][p.x] = TOCADO;
            ret = true;
        }
    }


    return ret;
}
