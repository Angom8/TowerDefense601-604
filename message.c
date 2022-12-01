#include "message.h"

/**
 * Initialiser un message
 * @param message Le message
 * @param type Le type
 * @return OK si réussite
 */
int initialiser_message(message_t * message, unsigned short int type){
	if(message != NULL){
		
		message->type = type;
		message->i = -1;
		message->j = -1;
		message->k = -1;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le message est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Initialiser un message avec un entier
 * @param message Le message
 * @param type Le type
 * @param i L'entier
 * @return OK si réussite
 */
int initialiser_message_entier(message_t * message, unsigned short int type, int i){
	if(message != NULL){
		
		message->type = type;
		message->i = i;
		message->j = -1;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le message est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Initialiser un message avec deux entiers
 * @param message Le message
 * @param type Le type
 * @param i L'entier 1
 * @param j L'entier 2
 * @return OK si réussite
 */
int initialiser_message_2_entiers(message_t * message, unsigned short int type, int i, int j){
	if(message != NULL){
		
		message->type = type;
		message->i = i;
		message->j = j;
		message->k = -1;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le message est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Initialiser un message avec deux entiers
 * @param message Le message
 * @param type Le type
 * @param i L'entier 1
 * @param j L'entier 2
 * @param k L'entier 3
 * @return OK si réussite
 */
int initialiser_message_3_entiers(message_t * message, unsigned short int type, int i, int j, int k){
	if(message != NULL){
		
		message->type = type;
		message->i = i;
		message->j = j;
		message->k = k;
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : Le message est nul\n");
	exit(EXIT_FAILURE);
}
