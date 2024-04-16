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

typedef struct {
    size_t      len;
    barco_dir_e dir;
    SDL_Point   pos;
} barco_t;

//static barco_t own_ships[BARCO_NUM_BARCOS];
//static barco_t enemy_ships[BARCO_NUM_BARCOS];

static bool own_ships[FILAS][COLUMNAS];


static bool        barco_crear(size_t len);
static barco_dir_e barco_rand_dir();
static void        barco_rand_pos(barco_dir_e dir, size_t len, int *c, int *f);
static bool        barco_validar(barco_dir_e dir, size_t len, int c, int f);



bool barco_crear(size_t len)
{
    bool        ret = false;
    int         c, f;
    barco_dir_e dir;

    dir = barco_rand_dir();
    barco_rand_pos(dir, len, &c, &f);

    ret = barco_validar(dir, len, c, f);

    if (ret == false) {
        fprintf(stderr, "error creando barco:\n");
        fprintf(stderr, "    dir: %d\n", dir);
        fprintf(stderr, "    len: %d\n", len);
        fprintf(stderr, "    f:   %d\n", f);
        fprintf(stderr, "    c:   %d\n", c);
    }

    return ret;
}

barco_dir_e barco_rand_dir()
{
    barco_dir_e ret;

    ret = (barco_dir_e) (rand() % 2);

    return ret;
}

void barco_rand_pos(barco_dir_e dir, size_t len, int *c, int *f)
{
    /* Poner las limitaciones en el rand nos permite forzar el sentido del
     * barco, lo que facilita a la hora de comprobar la validez. En vertical
     * el barco va hacia abajo desde barco.pos. En horizontal va hacia la
     * derecha */
    if (dir == VERTICAL) {
        *c = (int) (rand() % (COLUMNAS));
        *f = (int) (rand() % (FILAS - len));
    }
    else if (dir == HORIZONTAL) {
        *c = (int) (rand() % (COLUMNAS - len));
        *f = (int) (rand() % (FILAS));
    }
}

bool barco_validar(barco_dir_e dir, size_t len, int c, int f)
{
    size_t i;
    bool   ret = true;

    if (dir == VERTICAL) {
        for (i = 0; i < len; i++) {
            if (((i + f) < FILAS) && (own_ships[f + i][c] == 0)) {
                continue;
            }
            else {
                ret = false;
                break;
            }
        }
    }
    else if (dir == HORIZONTAL) {
         for (i = 0; i < len; i++){
             if (((i + c) < COLUMNAS) && (own_ships[f][c + i] == 0)) {
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

void barco_init()
{
    bool ret;
    int  f, c;
    srand((unsigned int) time(NULL));

    /* Inicializa matriz de barcos */
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLUMNAS; c++) {
            own_ships[f][c] = false;
        }
    }

    /* 2 barcos de 2, 1 de 3 y 1 de 4 */
    ret  = barco_crear((size_t) 2);
    ret |= barco_crear((size_t) 2);
    ret |= barco_crear((size_t) 3);
    ret |= barco_crear((size_t) 4);

    if (ret == false) {
        fprintf(stderr, "No se han podido inicializar los barcos.\n");
        exit(EXIT_FAILURE);
    }

}


