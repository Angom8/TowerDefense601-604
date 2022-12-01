#ifndef _ADRESSE_
#define _ADRESSE_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

typedef struct{
	struct sockaddr_in adresse;
}adresse_t;

/**
 * Créer l'adresse en utilisant une adresse sous forme de message
 * @param adresse L'adresse à créer
 * @param port Le port
 * @param adresse_msg L'adresse sous forme de message
 * @return OK si réussite
 */
int creer_adresse(adresse_t * adresse, unsigned short int port, char * adresse_msg);

/**
 * Créer l'adresse en utilisant une adresse sous forme d'entier
 * @param adresse L'adresse à créer
 * @param port Le port
 * @param adresse L'adresse sous forme d'entier
 * @return OK si réussite
 */
int creer_adresse_entier(adresse_t * adresse, unsigned short int port, unsigned int adresse_entier);

#endif
