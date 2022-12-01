#ifndef _ENTITE_
#define _ENTITE_

/* Types de case */
#define CASE_VIDE         0
#define CASE_MIN_JOUEUR   1
#define CASE_MAX_JOUEUR   3
#define CASE_MIN_CHEMIN   4
#define CASE_MAX_CHEMIN 229
#define CASE_TOUR		252
#define CASE_UNITE	253
#define CASE_ORDI       254
#define CASE_FORT       255

#include <pthread.h>

#include "tour.h"
#include "unite.h"

typedef struct{
	unsigned char type;
	unsigned short int x, y;
	pthread_t thread;
	tour_t tour;
	unite_t unite;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
}entite_t;

/**
 * Initialiser une entité
 * @param entite L'entité
 * @param type Le type
 * @param x La coordonnée x
 * @param y La coordonnée y
 * @return OK si réussite
 */
int initialiser_entite(entite_t * entite, unsigned char type, unsigned short int x, unsigned short int y);

#endif
