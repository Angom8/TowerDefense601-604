#include "tour.h"

/**
 * Creer une tour type 1
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_1(tour_t * tour){
	if(tour != NULL){
		tour->tir_min = TOUR_1_TIR_MIN;
		tour->tir_max = TOUR_1_TIR_MAX;
		tour->portee = TOUR_1_PORTEE;
		tour->vitesse = TOUR_1_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La tour est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer une tour type 2
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_2(tour_t * tour){
	if(tour != NULL){
		tour->tir_min = TOUR_2_TIR_MIN;
		tour->tir_max = TOUR_2_TIR_MAX;
		tour->portee = TOUR_2_PORTEE;
		tour->vitesse = TOUR_2_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La tour est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer une tour type 3
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_3(tour_t * tour){
	if(tour != NULL){
		tour->tir_min = TOUR_3_TIR_MIN;
		tour->tir_max = TOUR_3_TIR_MAX;
		tour->portee = TOUR_3_PORTEE;
		tour->vitesse = TOUR_3_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La tour est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer une tour type 4
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_4(tour_t * tour){
	if(tour != NULL){
		tour->tir_min = TOUR_4_TIR_MIN;
		tour->tir_max = TOUR_4_TIR_MAX;
		tour->portee = TOUR_4_PORTEE;
		tour->vitesse = TOUR_4_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La tour est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Creer une tour type 5
 * @param tour La tour
 * @return OK si réussite
 */
int creer_tour_5(tour_t * tour){
	if(tour != NULL){
		tour->tir_min = TOUR_5_TIR_MIN;
		tour->tir_max = TOUR_5_TIR_MAX;
		tour->portee = TOUR_5_PORTEE;
		tour->vitesse = TOUR_5_VITESSE;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La tour est nulle\n");
	exit(EXIT_FAILURE);
}
