#ifndef _COORD_
#define _COORD_

#include <stdlib.h>
#include <stdio.h>

typedef struct{
	unsigned short int x;
	unsigned short int y; 
}coord_t;

/**
 * Initialiser des coordonnées
 * @param coord Les coordonnées
 * @return OK si réussite
 */
int initialiser_coord(coord_t * coord, unsigned short int x, unsigned short int y);

#endif
