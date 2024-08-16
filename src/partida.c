#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "inc/partida.h"
#include "inc/barco.h"

/* ********************************************************************** */
#define NELEMS(x) ((sizeof (x)) / (sizeof ((x)[0])))


/* ********************************************************************** */
/* *********************** Declaración de tipos ************************* */
/* ********************************************************************** */
typedef enum {
    STATE_TURNO_ALIADO,
    STATE_TURNO_ENEMIGO
} partida_state_e;

typedef enum {
    EVENT_DISPARO_ALIADO,
    EVENT_DISPARO_ENEMIGO,
    EVENT_NONE
} partida_event_e;

typedef struct {
    partida_state_e from;
    partida_event_e event;
    partida_state_e to;
} partida_transition_t;

typedef struct {
    partida_state_e state;
    uint8_t         filas;
    uint8_t         cols;
    uint8_t         enemigos_restantes;
    uint8_t         aliados_restantes;
} partida_datos_t;


/* ************************* Functiones estáticas *********************** */
static void partida_hacer_transicion(partida_event_e event);





/* partida_sm_cb: tipo nuevo. Puntero a función que como parámetro toma un
 * puntero a struct partida_datos_t y devuelve enum partida_event_e */
typedef partida_event_e (* partida_sm_cb) (partida_datos_t *);


/* Datos de la máquina de estados */
static partida_datos_t partida_datos;

/* Array de transiciones */
static const partida_transition_t transitions[] = {
    {STATE_TURNO_ALIADO,  EVENT_DISPARO_ALIADO,  STATE_TURNO_ENEMIGO},
    {STATE_TURNO_ENEMIGO, EVENT_DISPARO_ENEMIGO, STATE_TURNO_ALIADO},
};



void partida_hacer_transicion(partida_event_e event)
{
    int i;

    for (i = 0; i < (int) NELEMS(transitions); i++) {
        if (partida_datos.state == transitions[i].from
                && event == transitions[i].event) {
            partida_datos.state = transitions[i].to;
            break;
        }
    }
    
    return;
}




void partida_init(uint8_t filas, uint8_t cols)
{
    int ret = 0;

    barco_init();

    /* 2 barcos de 2, 1 de 3 y 1 de 4 */
    /* Crear barcos */
    if (barco_crear((size_t) 2, ALIADO))  ret++;
    if (barco_crear((size_t) 2, ALIADO))  ret++;
    if (barco_crear((size_t) 3, ALIADO))  ret++;
    if (barco_crear((size_t) 4, ALIADO))  ret++;

    if (barco_crear((size_t) 2, ENEMIGO)) ret++;
    if (barco_crear((size_t) 2, ENEMIGO)) ret++;
    if (barco_crear((size_t) 3, ENEMIGO)) ret++;
    if (barco_crear((size_t) 4, ENEMIGO)) ret++;

    partida_datos.state = STATE_TURNO_ALIADO;
    partida_datos.filas = filas;
    partida_datos.cols  = cols;
    partida_datos.enemigos_restantes = 11; /* Número de casillas totales */

    if (ret != 8) {
        fprintf(stderr, "Error creando barcos\n");
        exit(EXIT_FAILURE);
    }

    return;
}

void partida_disparo_aliado(SDL_Point p)
{
    if (partida_datos.state != STATE_TURNO_ALIADO) {
        fprintf(stderr, "ERROR: Se ha llamado a partida_disparo_aliado en un"
                "estado que no es STATE_TURNO_ALIADO\n");
        exit(EXIT_FAILURE);
    }

    if (barco_dispara_celda(p) == true) {
        partida_datos.enemigos_restantes--;
    }

    partida_hacer_transicion(EVENT_DISPARO_ALIADO);

    return;
}

void partida_disparo_enemigo(SDL_Point p)
{
    int       f, c;
    SDL_Point aux_point;

    if (partida_datos.state != STATE_TURNO_ENEMIGO) {
        fprintf(stderr, "ERROR: Se ha llamado a partida_disparo_enemigo en un"
                "estado que no es STATE_TURNO_ENEMIGO\n");
        exit(EXIT_FAILURE);
    }

    f = (rand() % partida_datos.filas);
    c = (rand() % partida_datos.cols);

    aux_point.y = f;
    aux_point.x = c;

    if (barco_dispara_celda(aux_point) == true) {
        partida_datos.aliados_restantes--;
    }

    partida_hacer_transicion(EVENT_DISPARO_ENEMIGO);

    return;
}

bool partida_esta_acabada()
{
    bool ret;
    if ((partida_datos.enemigos_restantes == 0) ||
        (partida_datos.aliados_restantes == 0))
        ret = true;
    else
        ret = false;

    return ret;
}
