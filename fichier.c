#include "fichier.h"

/**
 * Ouvrir un fichier en lecture seulement
 * @param nom_fichier Le nom du fichier
 * @return Le fichier si réussite
 */
fichier_t ouvrir_fichier_lecture(char * nom_fichier){
	fichier_t fichier;
	if(nom_fichier != NULL){
		if((fichier.id = open(nom_fichier, O_RDONLY, S_IRUSR|S_IWUSR)) == -1){
			perror("Erreur d'ouverture du fichier "); 
			exit(EXIT_FAILURE);
		}
		
		return fichier;
	}
	
	fprintf(stderr, "Erreur : Le nom du fichier est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Retourner la taille d'un fichier
 * @param fichier Le fichier
 * @return La taille du fichier si réussite
 */
off_t recuperer_taille_fichier(fichier_t * fichier){
	off_t taille;
	if(fichier != NULL){
		
		if(lseek(fichier->id, 0, SEEK_SET) == -1){
			perror("Erreur lseek ");
			exit(EXIT_FAILURE);
		}
		
		if((taille = lseek(fichier->id, 0, SEEK_END)) == -1){
			perror("Erreur lseek ");
			exit(EXIT_FAILURE);
		}
		
		if(lseek(fichier->id, 0, SEEK_SET) == -1){
			perror("Erreur lseek ");
			exit(EXIT_FAILURE);
		}
		
		return taille;
	}
	
	fprintf(stderr, "Erreur : Le fichier est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire la description d'un fichier
 * @param fichier Le fichier
 * @param description La description à initialiser
 * @return OK si réussite
 */
int lire_fichier_description(fichier_t * fichier, char * description){
	size_t taille;
	
	if(fichier != NULL && description != NULL){
		
		/* Lire la taille de la description */
		if(read(fichier->id, &taille, sizeof(size_t)) == -1){
			perror("Erreur de lecture "); 
			exit(EXIT_FAILURE);
		}
		/* Vérifier si la taille ne depasse pas la taille max */
		if(taille > DESCRIPTION_MAX){
			fprintf(stderr, "Erreur : La taille de la description est supérieur à la taille max\n");
			exit(EXIT_FAILURE);
		}
		
		/* Lire la description du fichier */
		if(read(fichier->id, description, taille) == -1){
			perror("Erreur de lecture "); 
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le fichier ou le tableau de description est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire un unsigned char dans un fichier
 * @param fichier Le fichier
 * @param uc Le unsigned char à initialiser
 * @param OK si réussite
 */
int lire_fichier_unsigned_char(fichier_t * fichier, unsigned char * uc){
	if(fichier != NULL && uc != NULL){
		
		/* Lire le unsigned char dans le fichier */
		if(read(fichier->id, uc, sizeof(unsigned char)) == -1){
			perror("Erreur de lecture "); 
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le fichier ou le pointeur unsigned char est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire une entree dans un fichier
 * @param fichier Le fichier
 * @param entree L'entrée à initialiser
 * @return OK si réussite
 */
int lire_fichier_entree(fichier_t * fichier, entree_t * entree){
	if(fichier != NULL && entree != NULL){
		
		/* Lire le temps dans le fichier */
		if(read(fichier->id, &entree->temps, sizeof(long)) == -1){
			perror("Erreur de lecture "); 
			exit(EXIT_FAILURE);
		}
		
		/* Lire le type dans le fichier */
		lire_fichier_unsigned_char(fichier, &entree->type);
		
		if(entree->type == 0){
			/* Lire un message dans le fichier */
			if(read(fichier->id, entree->donnees, DONNEES_MAX) == -1){
				perror("Erreur de lecture "); 
				exit(EXIT_FAILURE);
			}
			entree->donnee = 0;
		}
		else{
			/* Lire unsigned int dans le fichier */
			if(read(fichier->id, &entree->donnee, sizeof(unsigned int)) == -1){
				perror("Erreur de lecture "); 
				exit(EXIT_FAILURE);
			}
			strcpy(entree->donnees, "\0");
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le fichier ou l'entree est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire une carte dans un fichier
 * @param carte La carte à initialiser avec les données du fichier
 * @param nom_fichier Le nom du fichier
 * @return OK si réussite
 */
int lire_carte(carte_t * carte, char * nom_fichier){
	int i, j;
	fichier_t fichier;
	char chemin_fichier[NOM_MAX] = "cartes/";
	
	if(carte != NULL && nom_fichier != NULL){
		strcat(chemin_fichier, nom_fichier);
		
		/* Ouverture du fichier */
		fichier = ouvrir_fichier_lecture(chemin_fichier);
		
		/* Lire la description */
		lire_fichier_description(&fichier, carte->description);
		
		/* Lire les cases */
		for(i=0 ; i<CASES_MAX ; i++){
			for(j=0 ; j<CASES_MAX ; j++){
				lire_fichier_unsigned_char(&fichier, &carte->cases[i][j]);
				
				if(carte->cases[i][j] == CASE_ORDI){
					initialiser_coord(&carte->spawn_ordinateur, j, i);
				}
				else if(carte->cases[i][j] >= CASE_MIN_JOUEUR && carte->cases[i][j] <= CASE_MAX_JOUEUR){
					initialiser_coord(&carte->spawn_adversaires[carte->cases[i][j]-1], i, j);
				}
			}
		}
		
		/* Fermeture du fichier */
		fermer_fichier(&fichier);
		
		/* Initialisation du nom du fichier */
		strcpy(carte->nom.nom, nom_fichier);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La carte ou le nom du fichier est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire un scénario dans un fichier
 * @param scenario Le scénario à initialiser avec les données du fichier
 * @param nom_fichier Le nom du fichier
 * @return OK si réussite
 */
int lire_scenario(scenario_t * scenario, char * nom_fichier){
	fichier_t fichier;
	char chemin_fichier[NOM_MAX] = "scenarios/";
	off_t taille_fichier, taille_cur_fichier;
	
	if(scenario != NULL && nom_fichier != NULL){
		strcat(chemin_fichier, nom_fichier);
		
		/* Ouverture du fichier */
		fichier = ouvrir_fichier_lecture(chemin_fichier);
		
		/* Récupérer la taille du fichier */
		taille_fichier = recuperer_taille_fichier(&fichier);
		
		/* Lire la description */
		lire_fichier_description(&fichier, scenario->description);
		
		scenario->n_entrees = 0;
		/* Lire les entrees */
		do{
			/* Récupère la taille courante */
			if((taille_cur_fichier = lseek(fichier.id, 0, SEEK_CUR)) == -1){
				perror("Erreur lseek ");
				exit(EXIT_FAILURE);
			}
			
			if(taille_cur_fichier < taille_fichier){
				lire_fichier_entree(&fichier, &scenario->entrees[scenario->n_entrees++]);
				
				/* Vérifier qu'on dépasse pas la taille max d'entrees */
				if(scenario->n_entrees > ENTREES_MAX){
					fprintf(stderr, "Erreur : Le nombre d'entrees est superieur au max\n");
					exit(EXIT_FAILURE);
				}
			}
		}while(taille_cur_fichier < taille_fichier);
		
		/* Fermeture du fichier */
		fermer_fichier(&fichier);
		
		/* Initialisation du nom du fichier */
		strcpy(scenario->nom.nom, nom_fichier);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le scenario ou le nom du fichier est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Ouvrir un fichier en lecture seulement
 * @param fichier Le fichier
 * @return OK si réussite
 */
int fermer_fichier(fichier_t * fichier){
	if(fichier != NULL){
		if(close(fichier->id) == -1){
			perror("Erreur fermeture du fichier ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le fichier est nul\n");
	exit(EXIT_FAILURE);
}
