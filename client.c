#include "jeu.h"
#include "fenetre.h"
#include "interface.h"
#include "socket.h"
#include "adresse.h"
#include "serveur_tcp.h"

#include "client.h"

/**
 * Initialiser un client
 * @param client Le client
 * @param port Le port
 * @param adresse L'adresse
 * @return OK si réussite
 */
int initialiser_client(client_t * client, unsigned short int port, char * adresse){
	if(client != NULL && adresse != NULL){
		
		/* Initialisation du mutex du client */
		if(pthread_mutex_init(&client->mutex, NULL) == -1){
			fprintf(stderr, "Erreur : Init mutex\n");
			exit(EXIT_FAILURE);
		}
		
		/* Création de la socket */
  		creer_socket_udp(&client->socket);

		/* Création de l'adresse du serveur udp */
		creer_adresse(&client->adresseServeur, port, adresse);
		
		return EXIT_SUCCESS;
	}
	
	exit(EXIT_FAILURE);
	fprintf(stderr, "Erreur : client ou adresse nulle\n");
}

/**
 * Créer une partie depuis un client
 * @param client Le client
 * @return OK si réussite
 */
int creer_partie_client(client_t * client){
	liste_noms_fichiers_t noms_cartes;
	liste_noms_fichiers_t noms_scenarios;
	message_t message;
	int carte, scenario;
	
	if(client != NULL){
		/* Initialisation du message */
		initialiser_message(&message, TYPE_DEMANDE_LISTE_CARTES);

		/* Envoi de la demande de la liste des cartes */
		envoyer_donnees_socket_udp(&client->socket, &client->adresseServeur, &message, sizeof(message_t));
		
		/* Attente de la reception de la liste des noms de cartes */
		recevoir_donnees_socket_udp(&client->socket, &noms_cartes, sizeof(liste_noms_fichiers_t));
		
		/* Afficher la liste des noms des fichiers cartes */
		printf("Choisir une carte :\n");
		afficher_liste_noms_fichiers(&noms_cartes);
		if(scanf("%d", &carte) == -1){
			perror("Erreur scanf ");
			exit(EXIT_FAILURE);
		}
		
		/* Initialisation du message */
		initialiser_message(&message, TYPE_DEMANDE_LISTE_SCENARIOS);

		/* Envoi de la demande de la liste des scénarios */
		envoyer_donnees_socket_udp(&client->socket, &client->adresseServeur, &message, sizeof(message_t));
		
		/* Attente de la reception de la liste des noms de cartes */
		recevoir_donnees_socket_udp(&client->socket, &noms_scenarios, sizeof(liste_noms_fichiers_t));
		
		/* Afficher la liste des noms des fichiers scenarios */
		printf("Choisir un scenario :\n");
		afficher_liste_noms_fichiers(&noms_scenarios);
		if(scanf("%d", &scenario) == -1){
			perror("Erreur scanf ");
			exit(EXIT_FAILURE);
		}
		
		/* Initialisation du message */
		initialiser_message_2_entiers(&message, TYPE_CHOIX_NOUVELLE_PARTIE, carte, scenario);
	
		/* Envoi du choix de la nouvelle partie */
		envoyer_donnees_socket_udp(&client->socket, &client->adresseServeur, &message, sizeof(message_t));
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Rejoindre une partie déjà existante
 * @param client Le client
 * @return OK si réussite
 */
int rejoindre_partie_client(client_t * client){
	message_t message;
	liste_noms_fichiers_t noms_parties;
	int partie;
	
	if(client != NULL){
		/* Initialisation du message */
		initialiser_message(&message, TYPE_DEMANDE_LISTE_SERVEURS);

		/* Envoi de la demande de la liste des serveurs */
		envoyer_donnees_socket_udp(&client->socket, &client->adresseServeur, &message, sizeof(message_t));
		
		/* Attente de la reception de la liste des noms des parties */
		recevoir_donnees_socket_udp(&client->socket, &noms_parties, sizeof(liste_noms_fichiers_t));
		
		/* Tester si il y a aucune partie de disponible (erreur) */
		if(noms_parties.n <= 0){
			fprintf(stderr, "Erreur : Il y a aucune partie disponible\n");
			exit(EXIT_FAILURE);
		}
		
		/* Afficher la liste des noms des parties */
		printf("Choisir une partie :\n");
		afficher_liste_noms_fichiers(&noms_parties);
		if(scanf("%d", &partie) == -1){
			perror("Erreur scanf ");
			exit(EXIT_FAILURE);
		}
		
		/* Initialisation du message */
		initialiser_message_entier(&message, TYPE_CHOIX_REJOINDRE_PARTIE, partie);
	
		/* Envoi du numéro de la partie que l'on veut rejoindre */
		envoyer_donnees_socket_udp(&client->socket, &client->adresseServeur, &message, sizeof(message_t));
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Choisir une action
 * @param client Le client
 * @return OK si réussite
 */
int choisir_action_client(client_t * client){
	int choix;
	
	if(client != NULL){
		printf("Choisir une action :\n");
		printf("1.Créer une nouvelle partie\n");
		printf("2.Rejoindre une partie\n");
		if(scanf("%d", &choix) == -1){
			perror("Erreur scanf ");
			exit(EXIT_FAILURE);
		}
		
		if(choix != 1 && choix != 2){
			fprintf(stderr, "Erreur : le choix est incorrect\n");
			exit(EXIT_FAILURE);
		}
		
		if(choix == 1){
			/* Création d'une nouvelle partie */
			creer_partie_client(client);
		}
		else{
			/* Rejoindre une partie existante */
			rejoindre_partie_client(client);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Connexion au serveur tcp depuis le client
 * @param client Le client
 * @return OK si réussite
 */
int connexion_serveur_tcp_client(client_t * client){
	message_t message;
	
	if(client != NULL){
		
		/* Recevoir le port du serveur tcp */
		recevoir_donnees_socket_udp(&client->socket, &message, sizeof(message_t));
		
		/* Fermeture de la socket du serveur udp */
		fermer_socket(&client->socket);
		
		if(message.i == 0){
			fprintf(stderr, "Erreur : Impossible de se connecter au serveur tcp\n");
			exit(EXIT_FAILURE);
		}
		
		/* Création de l'adresse du serveur tcp */
		creer_adresse_entier(&client->adresseServeur, (unsigned short int)message.i, client->adresseServeur.adresse.sin_addr.s_addr);
		
		/* Intialiser la socket en mode tcp */
		creer_socket_tcp(&client->socket);
		
		/* Connexion au serveur tcp */
		connexion_socket(&client->socket, &client->adresseServeur);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket ou l'adresse est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Attendre les clients
 * @param socket La socket
 * @return OK si réussite
 */
int attendre_adversaire_client(client_t * client){
	message_t message;
	unsigned short int fin = FALSE;
	
	if(client != NULL){
		while(fin == FALSE){
			recevoir_donnees_socket_tcp(&client->socket, &message, sizeof(message_t));
			
			switch(message.type){
				case TYPE_NB_JOUEURS:
					printf("Attendre des joueurs %d/%d ...\n", message.i, NB_CLIENTS);
					break;
				
				case TYPE_DEMARRER_PARTIE:
					fin = TRUE;
					break;
			}
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer au serveur que le client à perdu un point de vie
 * @param client Le client
 * @return OK si réussite
 */
int envoyer_perdre_vie_client(client_t * client){
	message_t message;
	
	if(client != NULL){
		
		if(pthread_mutex_lock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		initialiser_message(&message, TYPE_PERDRE_VIE);
		
		envoyer_donnees_socket_tcp(&client->socket, &message, sizeof(message_t));
		
		if(pthread_mutex_unlock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer la fin d'un client au serveur tcp
 * @param client Le client
 * @return OK si réussite
 */
int envoyer_fin_client(client_t * client){
	message_t message;
	
	if(client != NULL){
		
		if(pthread_mutex_lock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		initialiser_message(&message, TYPE_FIN_CLIENT);
		
		envoyer_donnees_socket_tcp(&client->socket, &message, sizeof(message_t));
		
		if(pthread_mutex_unlock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer un client
 * @param client Le client
 * @return OK si réussite
 */
int fermer_client(client_t * client){
	if(client != NULL){
		
		/* Fermer le jeu */
		fermer_jeu(&client->jeu);
	
		/* Fermeture de la socket */
		fermer_socket(&client->socket);
	
		/* Suppression de l'interface */
		interface_supprimer(&client->interface);
		
		/* Destruction du thread */
		pthread_cancel(client->thread_affichage);
		pthread_join(client->thread_affichage, NULL);
			
		/* Destruction du thread */
		pthread_cancel(client->thread_incrementer_freeze_unfreeze);
		pthread_join(client->thread_incrementer_freeze_unfreeze, NULL);
		
		/* Detruire le mutex du client */
		if(pthread_mutex_destroy(&client->mutex) == -1){
			fprintf(stderr, "Erreur : destruction mutex\n");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le client est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param client Le client
 * @param c la touche pressée
 * @param routine_tour La routine d'une tour
 * @param routine_unite La routine d'une unite
 */
void interface_main_client(client_t * client, int c, void *(*routine_tour)(void*), void *(*routine_unite)(void*)){
	int sourisX, sourisY, posX, posY, attributs, attaque = ATTAQUE_VIDE;
	message_t message;
	char buffer[255];

	if((c == KEY_MOUSE) && (souris_getpos(&sourisX, &sourisY, NULL) == OK)) {
		/* Gestion des actions de la souris */
		if(fenetre_getcoordonnees(client->interface.outils, sourisX, sourisY, &posX, &posY)) {
			if(pthread_mutex_lock(&client->jeu.mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			interface_outils(&client->interface, &client->jeu, posX, posY);
			
			if(pthread_mutex_unlock(&client->jeu.mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(fenetre_getcoordonnees(client->interface.attaques, sourisX, sourisY, &posX, &posY)) {
			if(pthread_mutex_lock(&client->jeu.mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			attaque = interface_attaques(&client->interface, &client->jeu, posX, posY);
			
			if(pthread_mutex_unlock(&client->jeu.mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		else if(fenetre_getcoordonnees(client->interface.carte, sourisX, sourisY, &posX, &posY)) {
			interface_carte(&client->interface, &client->jeu, posX, posY, routine_tour, routine_unite);
		}
	}
	
	/* --- Envoi de l'attaque (si besoin) --- */
	if(attaque != ATTAQUE_VIDE){
		switch(attaque){
			case ATTAQUE_SOLDAT:
				initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_UNITE, UNITE_SOLDAT, posX-25);
				break;
			
			case ATTAQUE_COMMANDO:
				initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_UNITE, UNITE_COMMANDO, posX-25);
				break;
			
			case ATTAQUE_VEHICULE:
				initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_UNITE, UNITE_VEHICULE, posX-25);
				break;
				
			case ATTAQUE_MISSILE:
				initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_UNITE, UNITE_MISSILE, posX-25);
				break;
				
			case ATTAQUE_CHAR:
				initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_UNITE, UNITE_CHAR, posX-25);
				break;
				
			case ATTAQUE_FREEZE:
				initialiser_message_2_entiers(&message, TYPE_ENVOI_ENTREES, TYPE_FREEZE, posX-25);
				break;
		}
		
		if(pthread_mutex_lock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		envoyer_donnees_socket_tcp(&client->socket, &message, sizeof(message_t));
		
		if(pthread_mutex_unlock(&client->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
	}
	
	/* --- Reception de message --- */
	
	if(pthread_mutex_lock(&client->mutex) == -1){
		fprintf(stderr, "Erreur : Lock mutex\n");
		exit(EXIT_FAILURE);
	}
	/* Reception des messages du serveur tcp en mode non bloquant */
	attributs = fcntl(client->socket.socketfd, F_GETFL);
	fcntl(client->socket.socketfd, F_SETFL, attributs | O_NONBLOCK);
    
	if(recevoir_donnees_non_bloquant_socket_tcp(&client->socket, &message, sizeof(message_t)) == 0){
		fcntl(client->socket.socketfd, F_SETFL, attributs);
		
		switch(message.type){
			case TYPE_ENVOI_ENTREES:
				switch(message.i){
					case TYPE_MESSAGE:
						recevoir_donnees_socket_tcp(&client->socket, &buffer, sizeof(buffer));
						wprintw(client->interface.infos->interieur, "\n%s", buffer);
						break;
				
					case TYPE_UNITE:
						switch(message.k){
							case CASE_ORDI:
								ajouter_unite_jeu(&client->jeu, message.j, client->jeu.carte.spawn_ordinateur.x, client->jeu.carte.spawn_ordinateur.y, routine_unite);
								wprintw(client->interface.infos->interieur, "\nAjout d'une nouvelle unite!");
								break;
							case 0:
							case 1:
							case 2:
								ajouter_unite_jeu(&client->jeu, message.j, client->jeu.carte.spawn_adversaires[message.k].x,
															 client->jeu.carte.spawn_adversaires[message.k].y, routine_unite);
								wprintw(client->interface.infos->interieur, "\nAjout d'une nouvelle unite!");
								break;
						}
						break;
			
					case TYPE_FREEZE:
						lancer_freeze_jeu(&client->jeu);
						break;
				
					case TYPE_ARGENT:
						modifier_argent_jeu(&client->jeu, message.j);
						wprintw(client->interface.infos->interieur, "\nReception de %d argents !", message.j);
						break;
				}
				break;
				
			case TYPE_PERDRE_VIE:
				if(message.i >= 0 && message.i < N_ADV){
					if(pthread_mutex_lock(&client->jeu.mutex) == -1){
						fprintf(stderr, "Erreur : Lock mutex\n");
						exit(EXIT_FAILURE);
					}
					
					if(client->jeu.adv[message.i] > 0){
						client->jeu.adv[message.i]--;
					}
				
					if(pthread_mutex_unlock(&client->jeu.mutex) == -1){
						fprintf(stderr, "Erreur : Unlock mutex\n");
						exit(EXIT_FAILURE);
					}
				}
				break;
		}
	}
	else{
		fcntl(client->socket.socketfd, F_SETFL, attributs);
	}
	if(pthread_mutex_unlock(&client->mutex) == -1){
		fprintf(stderr, "Erreur : Unlock mutex\n");
		exit(EXIT_FAILURE);
	}
}
