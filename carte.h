#ifndef _CARTE_
#define _CARTE_

#define CASE_MIN_JOUEUR 1
#define CASE_MAX_JOUEUR 3
#define CASE_ORDI       254
#define NB_ADVERSAIRES	3
#define CASES_MAX 	15
#define CARTES_MAX 	8

#include "nom.h"
#include "dossier.h"
#include "coord.h"

typedef struct{
	nom_fichier_t nom;
	char description[DESCRIPTION_MAX];
	unsigned char cases[CASES_MAX][CASES_MAX];
	coord_t spawn_ordinateur;
	coord_t spawn_adversaires[NB_ADVERSAIRES];
}carte_t;

typedef struct{
	carte_t cartes[CARTES_MAX];
	size_t n;
}liste_cartes_t;

#include "fichier.h"

/**
 * Créer une liste de cartes
 * @param cartes La liste de cartes à créer
 * @return OK si réussite
 */
int generer_liste_cartes(liste_cartes_t * cartes);

/**
 * Afficher une liste de cartes
 * @param cartes La liste de cartes
 * @return OK si réussite
 */
int afficher_liste_cartes(liste_cartes_t * cartes);

/**
 * Générer une liste de nom de fichiers à partir d'une liste de cartes
 * @param cartes La liste de cartes
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_cartes(liste_cartes_t * cartes, liste_noms_fichiers_t * fichiers);

#endif
