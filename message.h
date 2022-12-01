#ifndef _MESSAGE_
#define _MESSAGE_

#define TAILLE_MAX 256

/* Type */
#define TYPE_NB_JOUEURS 			0
#define TYPE_DEMARRER_PARTIE 			1
#define TYPE_DEMANDE_LISTE_CARTES 		2
#define TYPE_DEMANDE_LISTE_SCENARIOS 	3
#define TYPE_DEMANDE_LISTE_SERVEURS 	4
#define TYPE_CHOIX_NOUVELLE_PARTIE 		5
#define TYPE_CHOIX_REJOINDRE_PARTIE 	6
#define TYPE_ENVOI_ENTREES 			7
#define TYPE_ENVOI_PORT 			8
#define TYPE_PERDRE_VIE				9
#define TYPE_FIN_CLIENT				10

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct{
	unsigned short int type;
	int i;
	int j;
	int k;
}message_t;

/**
 * Initialiser un message
 * @param message Le message
 * @param type Le type
 * @return OK si réussite
 */
int initialiser_message(message_t * message, unsigned short int type);

/**
 * Initialiser un message avec un entier
 * @param message Le message
 * @param type Le type
 * @param i L'entier
 * @return OK si réussite
 */
int initialiser_message_entier(message_t * message, unsigned short int type, int i);

/**
 * Initialiser un message avec deux entiers
 * @param message Le message
 * @param type Le type
 * @param i L'entier 1
 * @param j L'entier 2
 * @return OK si réussite
 */
int initialiser_message_2_entiers(message_t * message, unsigned short int type, int i, int j);

/**
 * Initialiser un message avec deux entiers
 * @param message Le message
 * @param type Le type
 * @param i L'entier 1
 * @param j L'entier 2
 * @param k L'entier 3
 * @return OK si réussite
 */
int initialiser_message_3_entiers(message_t * message, unsigned short int type, int i, int j, int k);

#endif
