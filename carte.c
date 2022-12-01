#include "carte.h"

/**
 * Créer une liste de cartes
 * @param cartes La liste de cartes à créer
 * @return OK si réussite
 */
int generer_liste_cartes(liste_cartes_t * cartes){
	dossier_t dossier;
	if(cartes != NULL){
		
		ouvrir_dossier(&dossier, "cartes");
		
		cartes->n = 0;
		while((dossier.dir = readdir(dossier.d)) != NULL){
			if(dossier.dir->d_type == DT_REG){
				lire_carte(&cartes->cartes[cartes->n++], dossier.dir->d_name);
			}
		}
		
		fermer_dossier(&dossier);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de cartes est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Afficher une liste de cartes
 * @param cartes La liste de cartes
 * @return OK si réussite
 */
int afficher_liste_cartes(liste_cartes_t * cartes){
	int i;
	
	if(cartes != NULL){
		
		for(i=0 ; i<cartes->n ; i++){
			printf("%d.%s\n", i+1, cartes->cartes[i].nom.nom);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la liste de cartes est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Générer une liste de nom de fichiers à partir d'une liste de cartes
 * @param cartes La liste de cartes
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_cartes(liste_cartes_t * cartes, liste_noms_fichiers_t * fichiers){
	int i;
	if(cartes != NULL && fichiers != NULL){
		
		fichiers->n = cartes->n;
		for(i=0 ; i<cartes->n ; i++){
			fichiers->noms[i] = cartes->cartes[i].nom;
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de cartes ou de noms de fichiers est nulle\n");
	exit(EXIT_FAILURE);
}
