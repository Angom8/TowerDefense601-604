#include "unite.h"

/**
 * Creer un soldat en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_soldat_unite(unite_t * unite){
	if(unite != NULL){
		unite->type = UNITE_SOLDAT;
		unite->vie = SOLDAT_VIE;
		unite->vitesse = SOLDAT_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'unite est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer un commando en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_commando_unite(unite_t * unite){
	if(unite != NULL){
		unite->type = UNITE_COMMANDO;
		unite->vie = COMMANDO_VIE;
		unite->vitesse = COMMANDO_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'unite est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer un vehicule en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_vehicule_unite(unite_t * unite){
	if(unite != NULL){
		unite->type = UNITE_VEHICULE;
		unite->vie = VEHICULE_VIE;
		unite->vitesse = VEHICULE_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'unite est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer un missile en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_missile_unite(unite_t * unite){
	if(unite != NULL){
		unite->type = UNITE_MISSILE;
		unite->vie = MISSILE_VIE;
		unite->vitesse = MISSILE_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'unite est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer un char en unité
 * @param unite L'unité
 * @return OK si réussite
 */
int creer_char_unite(unite_t * unite){
	if(unite != NULL){
		unite->type = UNITE_CHAR;
		unite->vie = CHAR_VIE;
		unite->vitesse = CHAR_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'unite est nulle\n");
	exit(EXIT_FAILURE);
}
