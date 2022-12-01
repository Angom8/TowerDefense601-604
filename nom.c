#include "nom.h"

/**
 * Afficher une liste de noms de fichiers
 * @param noms_fichiers La liste de noms de fichiers
 * @return OK si réussite
 */
int afficher_liste_noms_fichiers(liste_noms_fichiers_t * noms_fichiers){
	int i;
	
	if(noms_fichiers != NULL){
		
		for(i=0 ; i<noms_fichiers->n ; i++){
			printf("%d.%s\n", i+1, noms_fichiers->noms[i].nom);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste des noms de fichiers est nulle\n");
	exit(EXIT_FAILURE);
}
