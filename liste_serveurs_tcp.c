#include "liste_serveurs_tcp.h"

/**
 * Initialiser une liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @return OK si réussite
 */
int initialiser_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs){
	size_t i;
	if(serveurs != NULL){
		
		for(i=0 ; i<NB_SERVEURS_TCP ; i++){
			serveurs->serveurs[i].etat = ETEINT;
		}
		
		if(pthread_mutex_init(&serveurs->serveurs[i].mutex, NULL) == -1){
			fprintf(stderr, "Erreur : Init mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de serveurs tcp est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Ajouter un serveur à la liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @param carte La carte
 * @param scenario Le scenario
 * @param routine_serveur La routine du serveur
 * @return Le port si réussite
 */
int ajouter_serveur_tcp_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs, carte_t * carte, scenario_t * scenario, void *(*routine_serveur)(void*)){
	size_t i;
	int port = 0;
	serveur_tcp_t * serveur;
	
	if(serveurs != NULL && carte != NULL && scenario != NULL){
		
		for(i=0 ; i<NB_SERVEURS_TCP && port == 0 ; i++){
			serveur = &serveurs->serveurs[i];
			
			if(pthread_mutex_lock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(serveur->etat == ETEINT){
				port = creer_serveur_tcp(serveur, carte, scenario, routine_serveur);
			}
			
			if(pthread_mutex_unlock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		return port;
	}
	
	fprintf(stderr, "Erreur : La liste de serveurs tcp ou la carte ou le scenario est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Générer une liste de nom de fichiers à partir d'une liste de serveurs tcp
 * @param serveurs La liste de serveurs
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_serveurs(liste_serveurs_tcp_t * serveurs, liste_noms_fichiers_t * fichiers){
	int i;
	char buffer[NOM_MAX];
	serveur_tcp_t * serveur;
	
	if(serveurs != NULL && fichiers != NULL){
		fichiers->n = 0;
		for(i=0 ; i<NB_SERVEURS_TCP ; i++){
			serveur = &serveurs->serveurs[i];
			if(pthread_mutex_lock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			if(serveur->etat == EN_ROUTE){
				fichiers->noms[fichiers->n] = serveur->carte->nom;
				
				strcat(fichiers->noms[fichiers->n].nom, " ");
				strcat(fichiers->noms[fichiers->n].nom, serveur->scenario->nom.nom);
				
				snprintf(buffer, NOM_MAX, " %ld/%d", serveur->n_clients, NB_CLIENTS);
				strcat(fichiers->noms[fichiers->n++].nom, buffer);
			}
			
			if(pthread_mutex_unlock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de serveurs ou de noms de fichiers est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer une liste des noms des serveurs tcp avec une socket en mode udp
 * @param socket La socket
 * @param adresse L'adresse
 * @param serveurs La liste de serveurs
 * @return OK si réussite
 */
int envoyer_liste_noms_serveurs_udp(socket_t * socket, adresse_t * adresse, liste_serveurs_tcp_t * serveurs){
	liste_noms_fichiers_t noms_fichiers;
	
	if(socket != NULL && adresse != NULL && serveurs != NULL){
		
		generer_liste_noms_fichiers_serveurs(serveurs, &noms_fichiers);
		
		/* Envoi de la liste des noms de serveurs */
		envoyer_donnees_socket_udp(socket, adresse, &noms_fichiers, sizeof(liste_noms_fichiers_t));
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse ou la liste de scenarios est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Récupérer le port d'un serveur tcp depuis une liste de serveurs tcp
 * @param serveurs La liste de serveurs
 * @param port Le port à récupérer
 * @param n Le numéro du serveur tcp
 * @return OK si réussite
 */
int recuperer_port_serveur_tcp(liste_serveurs_tcp_t * serveurs, int * port, int n){
	size_t i, j;
	unsigned short int test = 0;
	serveur_tcp_t * serveur;
	if(serveurs != NULL && port != NULL){
		if(n >= 0 && n < NB_SERVEURS_TCP){
			for(i=0, j=0 ; i<NB_SERVEURS_TCP && test == 0 ; i++){
				serveur = &serveurs->serveurs[i];
				
				if(pthread_mutex_lock(&serveur->mutex) == -1){
					fprintf(stderr, "Erreur : Lock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(serveur->etat == EN_ROUTE){
					if(n == j){
						if(serveur->n_clients < NB_CLIENTS){
							*port = serveur->adresse.adresse.sin_port;
							test = 1;
						}
						else{
							break;
						}
					}
					else{
						j++;
					}
				}
				
				if(pthread_mutex_unlock(&serveur->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
			}
		}
		
		return test;
	}
	
	fprintf(stderr, "Erreur : La liste de serveurs ou le port est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer tous les serveurs en marche d'une liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @return OK si réussite
 */
int fermer_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs){
	size_t i;
	
	if(serveurs != NULL){
		
		for(i=0 ; i<NB_SERVEURS_TCP ; i++){
			fermer_serveur_tcp(&serveurs->serveurs[i]);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La liste de serveurs tcp est nulle\n");
	exit(EXIT_FAILURE);
}
