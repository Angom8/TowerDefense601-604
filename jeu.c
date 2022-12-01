#include "jeu.h"

/**
 * Initialiser une partie
 * @param jeu Le jeu
 * @return OK si réussite
 */
int initialiser_jeu(jeu_t * jeu){
	int i, j;
	
	if(jeu != NULL){
		
		jeu->vies = VIES_JOUEURS;
		for(i=0 ; i<N_ADV ; i++){
			jeu->adv[i] = VIES_JOUEURS;
		}
		jeu->argent = ARGENT;
		jeu->freeze = 0;
		jeu->unfreeze = 0;
		jeu->isFreeze = OFF_FREEZE;
		
		if(pthread_mutex_init(&jeu->mutex, NULL) == -1){
			fprintf(stderr, "Erreur : Init mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(pthread_cond_init(&jeu->cond_freeze, NULL) == -1){
			fprintf(stderr, "Erreur : Init cond\n");
			exit(EXIT_FAILURE);
		}
		
		for(i=0 ; i<CASES_MAX ; i++){
			for(j=0 ; j<CASES_MAX ; j++){
				initialiser_entite(&jeu->entites[i][j], jeu->carte.cases[i][j], j, i);
			}
		}
		
		for(i=0 ; i<UNITES_ATTENTES ; i++){
			initialiser_entite(&jeu->unites[i], CASE_VIDE, 0, 0);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Modifier l'argent d'un jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int modifier_argent_jeu(jeu_t * jeu, int argent){
	if(jeu != NULL){
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(argent < 0){
			if(-argent > jeu->argent){
				if(pthread_mutex_unlock(&jeu->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				return EXIT_FAILURE;
			}
		}
		
		jeu->argent += argent;
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Incrementer la valeur de freeze et de unfreeze
 * @param jeu Le jeu
 * @return OK si réussite
 */
int incrementer_freeze_unfreeze(jeu_t * jeu){
	if(jeu != NULL){
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->freeze < VALUE_FREEZE_MAX){
			jeu->freeze++;
		}
		
		if(jeu->unfreeze < VALUE_FREEZE_MAX){
			jeu->unfreeze++;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Retirer une vie du joueur
 * @param jeu Le jeu
 * @return OK si réussite
 */
int retirer_vie_joueur_jeu(jeu_t * jeu){
	if(jeu != NULL){
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->vies > 0){
			jeu->vies--;
		}
		else{
			/* Perdu */
			if(pthread_mutex_unlock(&jeu->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			return EXIT_FAILURE;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Retirer une vie du joueur
 * @param jeu Le jeu
 * id L'id de l'adversaire
 * @return OK si réussite
 */
int retirer_vie_adversaires_jeu(jeu_t * jeu, unsigned short int id){
	if(jeu != NULL){
		
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->adv[id] > 1){
			jeu->adv[id]--;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Ajouter une tour dans un jeu
 * @param jeu Le jeu
 * @param type Le type de tour
 * @param argent L'argent à payer
 * @param posX La position x de la tour
 * @param posY La position y de la tour
 * @param routine_tour La routine d'une tour
 * @param OK si réussite
 */
int ajouter_tour_jeu(jeu_t * jeu, long type, int argent, int posX, int posY, void *(*routine_tour)(void*)){
	entite_t * entite;
	if(jeu != NULL){
		
		if(jeu->carte.cases[posY][posX] == CASE_VIDE){
			entite = &jeu->entites[posY][posX];
			if(pthread_mutex_lock(&entite->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(entite->type == CASE_VIDE){
				if(modifier_argent_jeu(jeu, -argent) == 0){
					entite->type = CASE_TOUR;
					switch(type){
						case OUTIL_TOUR_1:
							creer_tour_1(&entite->tour);
							break;
						case OUTIL_TOUR_2:
							creer_tour_2(&entite->tour);
							break;
						case OUTIL_TOUR_3:
							creer_tour_3(&entite->tour);
							break;
						case OUTIL_TOUR_4:
							creer_tour_4(&entite->tour);
							break;
						case OUTIL_TOUR_5:
							creer_tour_5(&entite->tour);
							break;
					}
			
					if(pthread_create(&entite->thread, NULL, routine_tour, (void *)entite) == -1){
						perror("Erreur creation de thread ");
						exit(EXIT_FAILURE);
					}
					
					if(pthread_mutex_unlock(&entite->mutex) == -1){
						fprintf(stderr, "Erreur : Unlock mutex\n");
						exit(EXIT_FAILURE);
					}
				
					return EXIT_SUCCESS;
				}
			}
			
			if(pthread_mutex_unlock(&entite->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		return EXIT_FAILURE;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Ajouter une unité dans un jeu
 * @param jeu Le jeu
 * @param type Le type d'unité
 * @param posX La position x de l'unité
 * @param posY La position y de l'unité
 * @param routine_unite La routine d'une unite
 * @param OK si réussite
 */
int ajouter_unite_jeu(jeu_t * jeu, long type, int posX, int posY, void *(*routine_unite)(void*)){
	entite_t * entite;
	size_t i;
	if(jeu != NULL){
		
		for(i=0 ; i<UNITES_ATTENTES ; i++){
			entite = &jeu->unites[i];
			if(pthread_mutex_lock(&entite->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(entite->type == CASE_VIDE){
				entite->type = CASE_UNITE;
				switch(type){
					case UNITE_SOLDAT:
						creer_soldat_unite(&entite->unite);
						break;
					case UNITE_COMMANDO:
						creer_commando_unite(&entite->unite);
						break;
					case UNITE_VEHICULE:
						creer_vehicule_unite(&entite->unite);
						break;
					case UNITE_MISSILE:
						creer_missile_unite(&entite->unite);
						break;
					case UNITE_CHAR:
						creer_char_unite(&entite->unite);
						break;
				}
				entite->x = posX;
				entite->y = posY;
		
				if(pthread_create(&entite->thread, NULL, routine_unite, (void *)entite) == -1){
					fprintf(stderr, "Erreur : creation de thread\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				return EXIT_SUCCESS;
			}
			
			if(pthread_mutex_unlock(&entite->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		return EXIT_FAILURE;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Tirer sur une unité depuis une tour
 * @param jeu Le jeu
 * @param entite La tour
 * @return OK si réussite
 */
int tirer_tour_jeu(jeu_t * jeu, entite_t * entite){
	int x, y, degats;
	tour_t * tour;
	entite_t * entite_tmp;
	void * resultat;
	if(jeu != NULL && entite != NULL){
		
		if(pthread_mutex_lock(&entite->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		tour = &entite->tour;
		
		for(x = entite->x - tour->portee ; x<tour->portee*2+1 ; x++){
			for(y = entite->y - tour->portee ; y<tour->portee*2+1 ; y++){
				if(x != entite->x && y != entite->y){
					if(x >= 0 && y >= 0 && x < CASES_MAX && y < CASES_MAX){
						entite_tmp = &jeu->entites[y][x];
						if(pthread_mutex_lock(&entite_tmp->mutex) == -1){
							fprintf(stderr, "Erreur : Lock mutex\n");
							exit(EXIT_FAILURE);
						}
					
						if(entite_tmp->type == CASE_UNITE){
						
							/* Attaquer l'unité */
							srand(time(NULL)*getpid());
							degats = (rand()%(tour->tir_max-tour->tir_min))+tour->tir_min;
							
							if(degats >= entite_tmp->unite.vie){
								do{
									pthread_cancel(entite_tmp->thread);
									pthread_join(entite_tmp->thread, &resultat);
								}while(resultat != PTHREAD_CANCELED);
								
								entite_tmp->type = jeu->carte.cases[y][x];
								
								/* Récupère l'argent de l'unité tuée */
								switch(entite_tmp->unite.type){
									case UNITE_SOLDAT:
										modifier_argent_jeu(jeu, SOLDAT_COUT);
										break;
										
									case UNITE_COMMANDO:
										modifier_argent_jeu(jeu, COMMANDO_COUT);
										break;
										
									case UNITE_VEHICULE:
										modifier_argent_jeu(jeu, VEHICULE_COUT);
										break;
										
									case UNITE_MISSILE:
										modifier_argent_jeu(jeu, MISSILE_COUT);
										break;
										
									case UNITE_CHAR:
										modifier_argent_jeu(jeu, CHAR_COUT);
										break;
								}
							}
							else{
								entite_tmp->unite.vie -= degats;
							}
							
							if(pthread_mutex_unlock(&entite_tmp->mutex) == -1){
								fprintf(stderr, "Erreur : Unlock mutex\n");
								exit(EXIT_FAILURE);
							}
							
							if(pthread_mutex_unlock(&entite->mutex) == -1){
								fprintf(stderr, "Erreur : Unlock mutex\n");
								exit(EXIT_FAILURE);
							}
							
							return EXIT_SUCCESS;
						}
					
						if(pthread_mutex_unlock(&entite_tmp->mutex) == -1){
							fprintf(stderr, "Erreur : Unlock mutex\n");
							exit(EXIT_FAILURE);
						}
					}
				}
			}
		}
		
		if(pthread_mutex_unlock(&entite->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_FAILURE;
	}
	
	fprintf(stderr, "Erreur : Le jeu ou l'entite est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Déplacer une unité sur le chemin
 * @param jeu Le jeu
 * @param entite L'unité
 * @return Le pointeur de la nouvelle entité si réussite
 */
entite_t * deplacer_unite_jeu(jeu_t * jeu, entite_t * entite, unsigned short int * fin){
	int x, y;
	unsigned short int x_tmp = 0, y_tmp = 0, val_tmp = 255;
	unsigned short int ucase, depart = 0;
	entite_t * entite_tmp;
	
	if(jeu != NULL && entite != NULL && fin != NULL){
		if(pthread_mutex_lock(&entite->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		/* Vérifier si l'unité est sur la case de depart */
		ucase = jeu->carte.cases[entite->y][entite->x];
		if(ucase == CASE_ORDI || (ucase >= CASE_MIN_JOUEUR && ucase <= CASE_MAX_JOUEUR)){
			depart = 1;
		}
		
		x = entite->x-1;
		y = entite->y;
		if(x >= 0 && y >= 0 && x < CASES_MAX && y < CASES_MAX){
			if(jeu->carte.cases[y][x] == CASE_FORT){
				*fin = 1;
				if(depart == 1)
					entite->type = CASE_VIDE;
				else
					entite->type = ucase;
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				if(pthread_cond_signal(&entite->cond) == -1){
					fprintf(stderr, "Erreur : Cond signal\n");
					exit(EXIT_SUCCESS);
				}
				
				return entite;
			}
			else if(jeu->carte.cases[y][x] >= CASE_MIN_CHEMIN && jeu->carte.cases[y][x] <= CASE_MAX_CHEMIN){
				if(depart){
					if(val_tmp > jeu->carte.cases[y][x]){
						val_tmp = jeu->carte.cases[y][x];
						x_tmp = x;
						y_tmp = y;
					}
				}
				else{
					if(jeu->carte.cases[entite->y][entite->x] == jeu->carte.cases[y][x]-1){
						x_tmp = x;
						y_tmp = y;
					}
				}
			}
		}
		
		x = entite->x+1;
		y = entite->y;
		if(x >= 0 && y >= 0 && x < CASES_MAX && y < CASES_MAX){
			if(jeu->carte.cases[y][x] == CASE_FORT){
				*fin = 1;
				if(depart == 1)
					entite->type = CASE_VIDE;
				else
					entite->type = ucase;
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				if(pthread_cond_signal(&entite->cond) == -1){
					fprintf(stderr, "Erreur : Cond signal\n");
					exit(EXIT_SUCCESS);
				}
				
				return entite;
			}
			else if(jeu->carte.cases[y][x] >= CASE_MIN_CHEMIN && jeu->carte.cases[y][x] <= CASE_MAX_CHEMIN){
				if(depart){
					if(val_tmp > jeu->carte.cases[y][x]){
						val_tmp = jeu->carte.cases[y][x];
						x_tmp = x;
						y_tmp = y;
					}
				}
				else{
					if(jeu->carte.cases[entite->y][entite->x] == jeu->carte.cases[y][x]-1){
						x_tmp = x;
						y_tmp = y;
					}
				}
			}
		}
		
		x = entite->x;
		y = entite->y-1;
		if(x >= 0 && y >= 0 && x < CASES_MAX && y < CASES_MAX){
			if(jeu->carte.cases[y][x] == CASE_FORT){
				*fin = 1;
				if(depart == 1)
					entite->type = CASE_VIDE;
				else
					entite->type = ucase;
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				if(pthread_cond_signal(&entite->cond) == -1){
					fprintf(stderr, "Erreur : Cond signal\n");
					exit(EXIT_SUCCESS);
				}
				
				return entite;
			}
			else if(jeu->carte.cases[y][x] >= CASE_MIN_CHEMIN && jeu->carte.cases[y][x] <= CASE_MAX_CHEMIN){
				if(depart){
					if(val_tmp > jeu->carte.cases[y][x]){
						val_tmp = jeu->carte.cases[y][x];
						x_tmp = x;
						y_tmp = y;
					}
				}
				else{
					if(jeu->carte.cases[entite->y][entite->x] == jeu->carte.cases[y][x]-1){
						x_tmp = x;
						y_tmp = y;
					}
				}
			}
		}
		
		x = entite->x;
		y = entite->y+1;
		if(x >= 0 && y >= 0 && x < CASES_MAX && y < CASES_MAX){
			if(jeu->carte.cases[y][x] == CASE_FORT){
				*fin = 1;
				if(depart == 1)
					entite->type = CASE_VIDE;
				else
					entite->type = ucase;
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				if(pthread_cond_signal(&entite->cond) == -1){
					fprintf(stderr, "Erreur : Cond signal\n");
					exit(EXIT_SUCCESS);
				}
				
				return entite;
			}
			else if(jeu->carte.cases[y][x] >= CASE_MIN_CHEMIN && jeu->carte.cases[y][x] <= CASE_MAX_CHEMIN){
				if(depart){
					if(val_tmp > jeu->carte.cases[y][x]){
						val_tmp = jeu->carte.cases[y][x];
						x_tmp = x;
						y_tmp = y;
					}
				}
				else{
					if(jeu->carte.cases[entite->y][entite->x] == jeu->carte.cases[y][x]-1){
						x_tmp = x;
						y_tmp = y;
					}
				}
			}
		}
		
		if(val_tmp != 255 || depart == 0){
			
			entite_tmp = &jeu->entites[y_tmp][x_tmp];
			if(pthread_mutex_lock(&entite_tmp->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(entite_tmp->type == CASE_UNITE){
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_cond_wait(&entite_tmp->cond, &entite_tmp->mutex) == -1){
					fprintf(stderr, "Erreur : Cond wait\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_mutex_lock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Lock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(entite_tmp->type != CASE_UNITE){
					entite_tmp->thread = entite->thread;
					
					entite_tmp->type = CASE_UNITE;
					if(depart == 1)
						entite->type = CASE_VIDE;
					else
						entite->type = ucase;
						
					entite_tmp->unite = entite->unite;
					
					if(pthread_mutex_unlock(&entite_tmp->mutex) == -1){
						fprintf(stderr, "Erreur : Unlock mutex\n");
						exit(EXIT_FAILURE);
					}
					
					if(pthread_mutex_unlock(&entite->mutex) == -1){
						fprintf(stderr, "Erreur : Unlock mutex\n");
						exit(EXIT_FAILURE);
					}
					
					if(pthread_cond_signal(&entite->cond) == -1){
						fprintf(stderr, "Erreur : Cond signal\n");
						exit(EXIT_SUCCESS);
					}
					
					return entite_tmp;
				}
			}
			else if(entite_tmp->type >= CASE_MIN_CHEMIN && entite_tmp->type <= CASE_MAX_CHEMIN){
				entite_tmp->thread = entite->thread;
				
				entite_tmp->type = CASE_UNITE;
				if(depart == 1)
					entite->type = CASE_VIDE;
				else
					entite->type = ucase;
					
				entite_tmp->unite = entite->unite;
				
				if(pthread_mutex_unlock(&entite_tmp->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_cond_signal(&entite->cond) == -1){
					fprintf(stderr, "Erreur : Cond signal\n");
					exit(EXIT_SUCCESS);
				}
				
				entite = entite_tmp;
				
				return entite_tmp;
			}
		
			if(pthread_mutex_unlock(&entite_tmp->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		if(pthread_mutex_unlock(&entite->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return entite;
	}
	
	fprintf(stderr, "Erreur : Le jeu ou l'entite ou le boolean est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Attendre sur une variable de condition le temps du freeze pour une tour
 * @param jeu Le jeu
 * @return OK si réussite
 */
int attendre_freeze_tour_jeu(jeu_t * jeu){
	struct timespec time;

	if(jeu != NULL){
		
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->isFreeze == ON_FREEZE){
			clock_gettime(CLOCK_REALTIME, &time);
			time.tv_sec += 5;
			
			if(pthread_cond_timedwait(&jeu->cond_freeze, &jeu->mutex, &time) == -1){
				fprintf(stderr, "Erreur : Cond wait\n");
				exit(EXIT_FAILURE);
			}
			
			jeu->isFreeze = OFF_FREEZE;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lancer un unfreeze des tours dans le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int lancer_unfreeze_jeu(jeu_t * jeu){
	if(jeu != NULL){
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->unfreeze >= VALUE_FREEZE_MAX){
			jeu->unfreeze = 0;
			
			if(pthread_mutex_unlock(&jeu->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(pthread_cond_broadcast(&jeu->cond_freeze) == -1){
				fprintf(stderr, "Erreur : Cond broadcast\n");
				exit(EXIT_SUCCESS);
			}
			
			return EXIT_SUCCESS;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_FAILURE;
	}
	
	fprintf(stderr, "Erreur le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lancer un freeze des tours dans le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int lancer_freeze_jeu(jeu_t * jeu){
	if(jeu != NULL){
		
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		jeu->isFreeze = ON_FREEZE;
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Test si le joueur est toujours en vie
 * @param jeu Le jeu
 * @return OK si le joueur est toujours en vie
 */
int test_en_vie_jeu(jeu_t * jeu){
	unsigned short int en_vie = 0;
	if(jeu != NULL){
		
		if(pthread_mutex_lock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(jeu->vies > 0){
			en_vie = 1;
		}
		
		if(pthread_mutex_unlock(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(en_vie == 1){
			return EXIT_SUCCESS;
		}
		else{
			return EXIT_FAILURE;
		}
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int fermer_jeu(jeu_t * jeu){
	size_t i, j;
	if(jeu != NULL){
		
		for(i=0 ; i<CASES_MAX ; i++){
			for(j=0 ; j<CASES_MAX ; j++){
				
				/* Destruction du thread */
				/*pthread_cancel(jeu->entites[i][j].thread);
				pthread_join(jeu->entites[i][j].thread, NULL);*/
				
				if(pthread_mutex_destroy(&jeu->entites[i][j].mutex) == -1){
					fprintf(stderr, "Erreur : destruction mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(pthread_cond_destroy(&jeu->entites[i][j].cond) == -1){
					fprintf(stderr, "Erreur : destruction mutex\n");
					exit(EXIT_FAILURE);
				}
			}
		}
		
		for(i=0 ; i<UNITES_ATTENTES ; i++){
			/* Destruction du thread */
			/*pthread_cancel(jeu->unites[i].thread);
			pthread_join(jeu->unites[i].thread, NULL);*/
				
			if(pthread_mutex_destroy(&jeu->unites[i].mutex) == -1){
				fprintf(stderr, "Erreur : destruction mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(pthread_cond_destroy(&jeu->unites[i].cond) == -1){
				fprintf(stderr, "Erreur : destruction mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		if(pthread_mutex_destroy(&jeu->mutex) == -1){
			fprintf(stderr, "Erreur : destruction mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(pthread_cond_destroy(&jeu->cond_freeze) == -1){
			fprintf(stderr, "Erreur : destruction mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le jeu est nul\n");
	exit(EXIT_FAILURE);
}
