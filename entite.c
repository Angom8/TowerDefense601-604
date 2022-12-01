#include "entite.h"

/**
 * Initialiser une entité
 * @param entite L'entité
 * @param type Le type
 * @param x La coordonnée x
 * @param y La coordonnée y
 * @return OK si réussite
 */
int initialiser_entite(entite_t * entite, unsigned char type, unsigned short int x, unsigned short int y){
	if(entite != NULL){
		
		entite->type = type;
		entite->x = x;
		entite->y = y;
		
		if(pthread_mutex_init(&entite->mutex, NULL) == -1){
			fprintf(stderr, "Erreur : Init mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(pthread_cond_init(&entite->cond, NULL) == -1){
			fprintf(stderr, "Erreur : Init cond\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'entite est nulle\n");
	exit(EXIT_FAILURE);
}
