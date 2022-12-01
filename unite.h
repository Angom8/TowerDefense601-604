#ifndef _UNITE_
#define _UNITE_

/* Unite */
#define UNITE_SOLDAT 	1
#define UNITE_COMMANDO 	2
#define UNITE_VEHICULE	3
#define UNITE_MISSILE	4
#define UNITE_CHAR	5

/* Constantes pour les vies des unités */
#define SOLDAT_VIE          5
#define COMMANDO_VIE       10
#define VEHICULE_VIE       20
#define MISSILE_VIE        50
#define CHAR_VIE          100

/* Constantes pour les déplacements des unités */
#define SOLDAT_VITESSE   1000
#define COMMANDO_VITESSE  900
#define VEHICULE_VITESSE  500
#define MISSILE_VITESSE   300
#define CHAR_VITESSE      400

typedef struct {
	unsigned short int type;
	int vie;
	int vitesse;
}unite_t;

#include <stdio.h>
#include <stdlib.h>

/**
 * Creer un soldat en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_soldat_unite(unite_t * unite);

/**
 * Creer un commando en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_commando_unite(unite_t * unite);

/**
 * Creer un vehicule en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_vehicule_unite(unite_t * unite);

/**
 * Creer un missile en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_missile_unite(unite_t * unite);

/**
 * Creer un char en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_char_unite(unite_t * unite);

#endif
