#ifndef _SCENARIO_
#define _SCENARIO_

#define ENTREES_MAX 200
#define DONNEES_MAX 255
#define SCENARIOS_MAX 8

#define TYPE_MESSAGE 0
#define TYPE_UNITE 1
#define TYPE_FREEZE 2
#define TYPE_ARGENT 3

#include "nom.h"
#include "dossier.h"

typedef struct{
	long temps;
	unsigned char type;
	unsigned int donnee;
	char donnees[DONNEES_MAX];
}entree_t;

typedef struct{
	nom_fichier_t nom;
	char description[DESCRIPTION_MAX];
	entree_t entrees[ENTREES_MAX];
	size_t n_entrees;
}scenario_t;

typedef struct{
	scenario_t scenarios[SCENARIOS_MAX];
	size_t n;
}liste_scenarios_t;

#include "fichier.h"

/**
 * Créer une liste de scenarios
 * @param cartes La liste de cartes à créer
 * @return OK si réussite
 */
int generer_liste_scenarios(liste_scenarios_t * scenarios);

/**
 * Afficher une liste de scenarios
 * @param scenarios La liste de scenarios
 * @return OK si réussite
 */
int afficher_liste_scenarios(liste_scenarios_t * scenarios);

/**
 * Générer une liste de nom de fichiers à partir d'une liste de scenarios
 * @param scenarios La liste de scenarios
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_scenarios(liste_scenarios_t * scenarios, liste_noms_fichiers_t * fichiers);

#endif
