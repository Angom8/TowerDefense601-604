#include "coord.h"

/**
 * Initialiser des coordonnées
 * @param coord Les coordonnées
 * @return OK si réussite
 */
int initialiser_coord(coord_t * coord, unsigned short int x, unsigned short int y){
	if(coord != NULL){
		
		coord->x = x;
		coord->y = y;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Les coordonnees sont nulles\n");
	exit(EXIT_FAILURE);
}
