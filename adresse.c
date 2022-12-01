#include "adresse.h"

/**
 * Créer l'adresse en utilisant une adresse sous forme de message
 * @param adresse L'adresse à créer
 * @param port Le port
 * @param adresse_msg L'adresse sous forme de message
 * @return OK si réussite
 */
int creer_adresse(adresse_t * adresse, unsigned short int port, char * adresse_msg){
	if(adresse != NULL && adresse_msg != NULL){
		
		/* Création de l'adresse */
		memset(&adresse->adresse, 0, sizeof(struct sockaddr_in));
		adresse->adresse.sin_family = AF_INET;
		adresse->adresse.sin_port = port;
		if(inet_pton(AF_INET, adresse_msg, &adresse->adresse.sin_addr) != 1) {
			perror("Erreur lors de la conversion de l'adresse ");
			exit(EXIT_FAILURE);
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : l'adresse est nulle\n");
	exit(EXIT_FAILURE);
}

/**
 * Créer l'adresse en utilisant une adresse sous forme d'entier
 * @param adresse L'adresse à créer
 * @param port Le port
 * @param adresse L'adresse sous forme d'entier
 * @return OK si réussite
 */
int creer_adresse_entier(adresse_t * adresse, unsigned short int port, unsigned int adresse_entier){
	if(adresse != NULL){
		
		/* Création de l'adresse */
		memset(&adresse->adresse, 0, sizeof(struct sockaddr_in));
		adresse->adresse.sin_family = AF_INET;
		adresse->adresse.sin_port = port;
		adresse->adresse.sin_addr.s_addr = adresse_entier;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : l'adresse est nulle\n");
	exit(EXIT_FAILURE);
}
