#include "dossier.h"

/**
 * Ouvrir un dossier
 * @param dossier Le dossier
 * @param nom_dossier Le nom du dossier
 * @return OK si réussite
 */
int ouvrir_dossier(dossier_t * dossier, char * nom_dossier){
	if(dossier != NULL){
		
		if((dossier->d = opendir(nom_dossier)) == NULL){
			perror("Erreur d'ouverture du dossier ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : le dossier est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer un dossier
 * @param dossier Le dossier
 * @return OK si réussite
 */
int fermer_dossier(dossier_t * dossier){
	if(dossier != NULL){
		
		if(closedir(dossier->d) == -1){
			perror("Erreur fermeture du dossier ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : le dossier est nul\n");
	exit(EXIT_FAILURE);
}
