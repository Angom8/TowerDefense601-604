#ifndef _TOUR_
#define _TOUR_

/* Constantes pour les dégâts des tirs des tourelles */
#define TOUR_1_TIR_MIN      1
#define TOUR_1_TIR_MAX     10
#define TOUR_1_PORTEE       1
#define TOUR_1_VITESSE    500

#define TOUR_2_TIR_MIN      5
#define TOUR_2_TIR_MAX     20
#define TOUR_2_PORTEE       2
#define TOUR_2_VITESSE    400

#define TOUR_3_TIR_MIN     10
#define TOUR_3_TIR_MAX     40
#define TOUR_3_PORTEE       3
#define TOUR_3_VITESSE    300

#define TOUR_4_TIR_MIN     20
#define TOUR_4_TIR_MAX    100
#define TOUR_4_PORTEE       4
#define TOUR_4_VITESSE    200

#define TOUR_5_TIR_MIN     50
#define TOUR_5_TIR_MAX    200
#define TOUR_5_PORTEE       5
#define TOUR_5_VITESSE    100

#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int tir_min;
	int tir_max;
	int portee;
	int vitesse;
}tour_t;

/**
 * Creer une tour type 1
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_1(tour_t * tour);

/**
 * Creer une tour type 2
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_2(tour_t * tour);

/**
 * Creer une tour type 3
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_3(tour_t * tour);

/**
 * Creer une tour type 4
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_4(tour_t * tour);

/**
 * Creer une tour type 5
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_5(tour_t * tour);

#endif
