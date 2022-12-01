#include "serveur_tcp.h"

/**
 * Créer un serveur tcp 
 * @param serveur Le serveur tcp
 * @param carte La carte
 * @param scenario Le scenario
 * @param routine_serveur La routine du serveur
 * @return Le port si réussite
 */
int creer_serveur_tcp(serveur_tcp_t * serveur, carte_t * carte, scenario_t * scenario, void *(*routine_serveur)(void*)){
	size_t i;
	int port;
	if(serveur != NULL && carte != NULL && scenario != NULL){
		
		srand(time(NULL)*getpid());
		port = rand()%PORTS_MAX;
		
		creer_socket_tcp(&serveur->socketserveur);
		creer_adresse_entier(&serveur->adresse, port, htonl(INADDR_ANY));
		nommage_socket(&serveur->socketserveur, &serveur->adresse);
		mode_passif_socket(&serveur->socketserveur);
		
		for(i=0 ; i<NB_CLIENTS ; i++){
			serveur->vies[i] = 0;
		}
		
		serveur->n_clients = 0;
		serveur->carte = carte;
		serveur->scenario = scenario;
		serveur->n_scenario = 0;
		serveur->etat = EN_ROUTE;
		
		if(pthread_create(&serveur->thread, NULL, routine_serveur, (void *)serveur) == -1){
			perror("Erreur creation de thread ");
			exit(EXIT_FAILURE);
		}
		
		return port;
	}
	
	fprintf(stderr, "Erreur : Le serveur ou la carte ou le scenario est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Attendre les clients du serveur tcp
 * @param serveur Le serveur
 * @return OK si réussite
 */
int attendre_clients_serveur_tcp(serveur_tcp_t * serveur){
	message_t message;
	socket_t socket_tmp;
	int attributs;
	size_t i;
	
	if(serveur != NULL){
	
		message.type = TYPE_NB_JOUEURS;
		
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		while(serveur->n_clients < NB_CLIENTS){
			
			if(pthread_mutex_unlock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			socket_tmp = attendre_connexion_socket_client(&serveur->socketserveur);
			
			if(pthread_mutex_lock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			serveur->socketclients[serveur->n_clients] = socket_tmp;
			serveur->vies[serveur->n_clients] = VIES_JOUEURS;
			
			/* Envoie de la carte */
			envoyer_donnees_socket_tcp(&serveur->socketclients[serveur->n_clients++], serveur->carte, sizeof(carte_t));
			
			if(serveur->n_clients < NB_CLIENTS){
				/* Envoyer le nombre de clients connectés */
				message.i = serveur->n_clients;
				envoyer_donnees_clients_serveur_tcp(serveur, &message, sizeof(message_t));
			}
		}
		
		/* Envoyer démarrage de la partie */
		message.type = TYPE_DEMARRER_PARTIE;
		message.i = serveur->n_clients;
		envoyer_donnees_clients_serveur_tcp(serveur, &message, sizeof(message_t));
		
		serveur->etat = EN_JEU;
		
		/* Placer en non bloquant les sockets clients */
		for(i=0 ; i<NB_CLIENTS ; i++){
			attributs = fcntl(serveur->socketclients[i].socketfd, F_GETFL);
			fcntl(serveur->socketclients[i].socketfd, F_SETFL, attributs | O_NONBLOCK);
		}
		
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer des données aux clients du serveur tcp
 * @param serveur Le serveur
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_clients_serveur_tcp(serveur_tcp_t * serveur, void * donnees, size_t taille){
	size_t i;
	if(serveur != NULL){
		
		for(i=0 ; i<NB_CLIENTS ; i++){
			if(serveur->vies[i] > 0)
				envoyer_donnees_socket_tcp(&serveur->socketclients[i], donnees, taille);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer aux autres clients qu'un client à perdu un point de vie
 * @param serveur Le serveur tcp
 * @param n Le numéro du client qui à perdu un point de vie
 * @return OK si réussite
 */
int envoyer_perdre_vie_clients_serveur_tcp(serveur_tcp_t * serveur, unsigned short int n){
	size_t i;
	message_t message;
	
	if(serveur != NULL){
		
		for(i=0 ; i<NB_CLIENTS ; i++){
			if(i != n){
				if(i > n){
					initialiser_message_entier(&message, TYPE_PERDRE_VIE, n);
				}
				else if(i < n){
					initialiser_message_entier(&message, TYPE_PERDRE_VIE, n-1);
				}
				
				envoyer_donnees_socket_tcp(&serveur->socketclients[i], &message, sizeof(message_t));
			}
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Lire le message des clients depuis un serveur tcp
 * @param serveur Le serveur tcp
 * @return OK si réussite
 */
int lire_message_clients_serveur_tcp(serveur_tcp_t * serveur){
	size_t i;
	message_t message;
	int n;
	
	if(serveur != NULL){
		
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		for(i=0 ; i<NB_CLIENTS ; i++){
			if(serveur->vies[i] > 0 && recevoir_donnees_non_bloquant_socket_tcp(&serveur->socketclients[i], &message, sizeof(message_t)) == 0){
				switch(message.type){
					case TYPE_PERDRE_VIE:
						envoyer_perdre_vie_clients_serveur_tcp(serveur, i);
						serveur->vies[i]--;
						break;
						
					case TYPE_FIN_CLIENT:
						serveur->vies[i] = 0;
						break;
						
					case TYPE_ENVOI_ENTREES:
						if(i == message.k){
							message.k++;
						}
						n = message.k;
						
						if(message.k < i){
							message.k = i-1;
						}
						
						envoyer_donnees_socket_tcp(&serveur->socketclients[n], &message, sizeof(message_t));
						
						break;
				}
			}
		}
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Tester si les 4 joueurs sont encore en vie depuis un serveur tcp
 * @param serveur Le serveur tcp
 * @return OK si au moins un joueur est encore en vie
 */
int test_joueurs_en_vie_serveur_tcp(serveur_tcp_t * serveur){
	size_t i;
	unsigned short int en_vie = 0;
	if(serveur != NULL){
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		for(i=0 ; i<NB_CLIENTS && en_vie == 0 ; i++){
			if(serveur->vies[i] > 0){
				en_vie = 1;
			}
		}
		
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(en_vie == 1){
			return EXIT_SUCCESS;
		}else{
			return EXIT_FAILURE;
		}
	}
	
	fprintf(stderr, "Erreur : Le serveur tcp est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Eteindre le serveur tcp en route
 * @param serveur Le serveur
 * @return OK si réussite
 */
int eteindre_serveur_tcp(serveur_tcp_t * serveur){
	size_t i;
	
	if(serveur != NULL){
	
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(serveur->etat == EN_JEU || serveur->etat == EN_ROUTE){
			
			/* Fermer le thread scenario */
			pthread_cancel(serveur->thread_scenario);
			pthread_join(serveur->thread_scenario, NULL);
									
			/* Ferme la socket du serveur tcp */
			fermer_socket(&serveur->socketserveur);
		
			for(i=0 ; i<serveur->n_clients ; i++){
				/* Ferme la socket des clients */
				fermer_socket(&serveur->socketclients[i]);
			}
		
			/* Mode éteint */
			serveur->etat = ETEINT;
		}
		
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer le serveur tcp
 * @param serveur Le serveur
 * @return OK si réussite
 */
int fermer_serveur_tcp(serveur_tcp_t * serveur){
	size_t i;
	
	if(serveur != NULL){
	
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(serveur->etat == EN_JEU || serveur->etat == EN_ROUTE){
			/* Fermer le thread de base du serveur tcp */
			pthread_cancel(serveur->thread);
			pthread_join(serveur->thread, NULL);
			
			/* Fermer le thread scenario */
			pthread_cancel(serveur->thread_scenario);
			pthread_join(serveur->thread_scenario, NULL);
									
			/* Ferme la socket du serveur tcp */
			fermer_socket(&serveur->socketserveur);
		
			for(i=0 ; i<serveur->n_clients ; i++){
				/* Ferme la socket des clients */
				fermer_socket(&serveur->socketclients[i]);
			}
		
			/* Mode éteint */
			serveur->etat = ETEINT;
		}
		
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		if(pthread_mutex_destroy(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : destruction mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le serveur est nul\n");
	exit(EXIT_FAILURE);
}
