#include <stdlib.h>    /* size_t, rand, srand */
#include <time.h>      /* time                */
#include <stdio.h>     /* NULL                */
#include <stdbool.h>   /* true, false         */

#include <SDL2/SDL.h>  /* SDL_Point           */

#include "inc/hlf.h"
#include "inc/barco.h"

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

    return ret;;
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
        *c = rand() % (COLUMNAS);
        *f = rand() % (FILAS - len);
    }
    else if (dir == HORIZONTAL) {
        *c = rand() % (COLUMNAS - len);
        *f = rand() % (FILAS);
    }
}

bool barco_validar(barco_dir_e dir, size_t len, int c, int f)
{
    size_t i;

    for (i = 0; i < len; i++) {

    }
}

void barco_init()
{
    bool ret;
    srand((unsigned int) time(NULL));


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


