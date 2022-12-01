#include "socket.h"

/**
 * Créer une socket en mode udp
 * @param socket Le socket
 * @return OK si réussite
 */
int creer_socket_udp(socket_t * s){
	if(s != NULL){
		
		/* Création de la socket en mode udp */
		if((s->socketfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
			perror("Erreur lors de la création de la socket ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Créer une socket en mode tcp
 * @param socket La socket
 * @return OK si réussite
 */
int creer_socket_tcp(socket_t * s){
	if(s != NULL){
		
		/* Création de la socket en mode tcp */
		if((s->socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
			perror("Erreur lors de la création de la socket ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Nommer la socket
 * @param socket La socket
 * @param adresse L'adresse
 * @return OK si réussite
 */
int nommage_socket(socket_t * socket, adresse_t * adresse){
	if(socket != NULL && adresse != NULL){
		
		/* Nommage de la socket */
		if(bind(socket->socketfd, (struct sockaddr*)&adresse->adresse, sizeof(struct sockaddr_in)) == -1) {
			perror("Erreur lors du nommage de la socket ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Mettre en mode passif
 * @param socket La socket
 * @return OK si réussite
 */
int mode_passif_socket(socket_t * socket){
	if(socket != NULL){
		
		/* Mise en mode passif de la socket */
		if(listen(socket->socketfd, 1) == -1) {
			perror("Erreur lors de la mise en mode passif ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Connexion au serveur
 * @param socket La socket
 * @param adresse L'adresse
 * @return OK si réussite
 */
int connexion_socket(socket_t * socket, adresse_t * adresse){
	if(socket != NULL && adresse != NULL){
		
		/* Connexion au serveur */
		if(connect(socket->socketfd, (struct sockaddr*)&adresse->adresse, sizeof(struct sockaddr_in)) == -1) {
			perror("Erreur lors de la connexion ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Attendre une connexion
 * @param socket La socket
 * @return La socket du client si réussite
 */
socket_t attendre_connexion_socket_client(socket_t * socket){
	socket_t sclient;
	if(socket != NULL){
		
		/* Attente d'une connexion */
		if((sclient.socketfd = accept(socket->socketfd, NULL, NULL)) == -1) {
			perror("Erreur lors de la demande de connexion ");
			exit(EXIT_FAILURE);
		}
		
		return sclient;
	}
	
	fprintf(stderr, "Erreur : la socket est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer des données avec une socket en mode udp
 * @param socket La socket
 * @param adresse L'adresse
 * @param donnees Les données à envoyer
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_socket_udp(socket_t * socket, adresse_t * adresse, void * donnees, size_t taille){
	if(socket != NULL && adresse != NULL && donnees != NULL){
		
		/* Envoi des données */
		if(sendto(socket->socketfd, donnees, taille, 0, (struct sockaddr*)&adresse->adresse, sizeof(struct sockaddr_in)) == -1) {
			perror("Erreur lors de l'envoi des donnees ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse ou les donnees sont nulles\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer une liste des noms de cartes avec une socket
 * @param socket La socket
 * @param adresse L'adresse
 * @param cartes La liste de cartes
 * @return OK si réussite
 */
int envoyer_liste_noms_cartes_udp(socket_t * socket, adresse_t * adresse, liste_cartes_t * cartes){
	liste_noms_fichiers_t noms_fichiers;
	
	if(socket != NULL && adresse != NULL && cartes != NULL){
		
		generer_liste_noms_fichiers_cartes(cartes, &noms_fichiers);
		
		/* Envoi de la liste des noms de cartes */
		envoyer_donnees_socket_udp(socket, adresse, &noms_fichiers, sizeof(liste_noms_fichiers_t));
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse ou la liste de cartes est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer une liste des noms de scenarios avec une socket
 * @param socket La socket
 * @param adresse L'adresse
 * @param scenarios La liste de scenarios
 * @return OK si réussite
 */
int envoyer_liste_noms_scenarios_udp(socket_t * socket, adresse_t * adresse, liste_scenarios_t * scenarios){
	liste_noms_fichiers_t noms_fichiers;
	
	if(socket != NULL && adresse != NULL && scenarios != NULL){
		
		generer_liste_noms_fichiers_scenarios(scenarios, &noms_fichiers);
		
		/* Envoi de la liste des noms de scénarios */
		envoyer_donnees_socket_udp(socket, adresse, &noms_fichiers, sizeof(liste_noms_fichiers_t));
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket ou l'adresse ou la liste de scenarios est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Envoyer des données avec une socket en mode tcp
 * @param socket La socket
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_socket_tcp(socket_t * socket, void * donnees, size_t taille){
	if(socket != NULL && donnees != NULL){
		
		if(write(socket->socketfd, donnees, taille) == -1){
			perror("Erreur lors de l'envoi des donnees ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket ou les donnees sont nulles\n");
	exit(EXIT_FAILURE);
}

/**
 * Recevoir des données depuis une socket en mode udp
 * @param socket La socket
 * @param donnees Les données à recevoir
 * @param taille La taille des données
 * @return L'adresse de l'envoyeur
 */
adresse_t recevoir_donnees_socket_udp(socket_t * socket, void * donnees, size_t taille){
	adresse_t adresse;
	socklen_t taille_adresse = sizeof(struct sockaddr_in);
	
	if(socket != NULL && donnees != NULL){
		
		/* Attendre de recevoir les données */
		if(recvfrom(socket->socketfd, donnees, taille, 0, (struct sockaddr*)&adresse.adresse, &taille_adresse) == -1) {
			if(errno != EINTR){
				perror("Erreur lors de la réception des donnees ");
				exit(EXIT_FAILURE);
			}
		}
		
		return adresse;
	}
	
	fprintf(stderr, "Erreur : la socket ou les donnees sont nulles\n");
	exit(EXIT_FAILURE);
}

/**
 * Recevoir des données depuis une socket client en mode tcp
 * @param sclient La socket client
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int recevoir_donnees_socket_tcp(socket_t * socket, void * donnees, size_t taille){
	if(socket != NULL && donnees != NULL){
		
		/* Lecture des donnees */
		if(read(socket->socketfd, donnees, taille) == -1){
			if(errno != EINTR){
				perror("Erreur lors de la lecture des donnees ");
				exit(EXIT_FAILURE);
			}
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket ou les donnees sont nulles\n");
	exit(EXIT_FAILURE);
}

/**
 * Recevoir des données en mode non bloquant depuis une socket client en mode tcp
 * @param socket La socket client
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int recevoir_donnees_non_bloquant_socket_tcp(socket_t * socket, void * donnees, size_t taille){
	if(socket != NULL && donnees != NULL){
		
		/* Lecture des donnees */
		if(read(socket->socketfd, donnees, taille) == -1) {
			if(errno != EAGAIN){
				perror("Erreur lors de la lecture des donnees ");
				exit(EXIT_FAILURE);
			}
			else{
				return EXIT_FAILURE;
			}
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket ou les donnees sont nulles\n");
	exit(EXIT_FAILURE);
}

/**
 * Fermer une socket
 * @param socket La socket
 * @return OK si réussite
 */
int fermer_socket(socket_t * socket){
	if(socket != NULL){
		
		if(close(socket->socketfd) == -1) {
			perror("Erreur lors de la fermeture de la socket ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : la socket est nulle\n");
	exit(EXIT_FAILURE);
}
