#include "scenario.h"

/**
 * Créer une liste de scenarios
 * @param cartes La liste de cartes à créer
 * @return OK si réussite
 */
int generer_liste_scenarios(liste_scenarios_t * scenarios){
	dossier_t dossier;
	if(scenarios != NULL){
		
		ouvrir_dossier(&dossier, "scenarios");
		
		scenarios->n = 0;
		while((dossier.dir = readdir(dossier.d)) != NULL){
			if(dossier.dir->d_type == DT_REG){
				lire_scenario(&scenarios->scenarios[scenarios->n++], dossier.dir->d_name);
			}
		}
		
		fermer_dossier(&dossier);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de scenarios est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Afficher une liste de scenarios
 * @param scenarios La liste de scenarios
 * @return OK si réussite
 */
int afficher_liste_scenarios(liste_scenarios_t * scenarios){
	int i;
	
	if(scenarios != NULL){
		
		for(i=0 ; i<scenarios->n ; i++){
			printf("%d.%s\n", i+1, scenarios->scenarios[i].nom.nom);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la liste de scenarios est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Générer une liste de nom de fichiers à partir d'une liste de scenarios
 * @param scenarios La liste de scenarios
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_scenarios(liste_scenarios_t * scenarios, liste_noms_fichiers_t * fichiers){
	int i;
	if(scenarios != NULL && fichiers != NULL){
		
		fichiers->n = scenarios->n;
		for(i=0 ; i<scenarios->n ; i++){
			fichiers->noms[i] = scenarios->scenarios[i].nom;
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de scenarios ou de noms de fichiers est nulle\n");
	exit(EXIT_FAILURE);
}
