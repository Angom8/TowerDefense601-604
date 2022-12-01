#ifndef _DOSSIER_
#define _DOSSIER_

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

typedef struct{
	DIR * d;
	struct dirent * dir;
}dossier_t;

/**
 * Ouvrir un dossier
 * @param dossier Le dossier
 * @param nom_dossier Le nom du dossier
 * @return OK si réussite
 */
int ouvrir_dossier(dossier_t * dossier, char * nom_dossier);

/**
 * Fermer un dossier
 * @param dossier Le dossier
 * @return OK si réussite
 */
int fermer_dossier(dossier_t * dossier); 

#endif
