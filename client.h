#ifndef _CLIENT_
#define _CLIENT_

typedef struct{
	jeu_t jeu;
	interface_t interface;
	socket_t socket;
	adresse_t adresseServeur;
	pthread_t thread_affichage;
	pthread_t thread_incrementer_freeze_unfreeze;
	pthread_mutex_t mutex;
}client_t;

/**
 * Initialiser un client
 * @param client Le client
 * @param port Le port
 * @param adresse L'adresse
 * @return OK si réussite
 */
int initialiser_client(client_t * client, unsigned short int port, char * adresse);

/**
 * Choisir une action
 * @param client Le client
 * @return OK si réussite
 */
int choisir_action_client(client_t * client);

/**
 * Créer une partie depuis un client
 * @param client Le client
 * @return OK si réussite
 */
int creer_partie_client(client_t * client);

/**
 * Rejoindre une partie déjà existante
 * @param client Le client
 * @return OK si réussite
 */
int rejoindre_partie_client(client_t * client);

/**
 * Connexion au serveur tcp depuis le client
 * @param client Le client
 * @return OK si réussite
 */
int connexion_serveur_tcp_client(client_t * client);

/**
 * Attendre les clients
 * @param socket La socket
 * @return OK si réussite
 */
int attendre_adversaire_client(client_t * client);

/**
 * Envoyer au serveur que le client à perdu un point de vie
 * @param client Le client
 * @return OK si réussite
 */
int envoyer_perdre_vie_client(client_t * client);

/**
 * Envoyer la fin d'un client au serveur tcp
 * @param client Le client
 * @return OK si réussite
 */
int envoyer_fin_client(client_t * client);

/**
 * Fermer un client
 * @param client Le client
 * @return OK si réussite
 */
int fermer_client(client_t * client);

/**
 * Gestion des actions de l'utilisateur dans l'interface.
 * @param client Le client
 * @param c la touche pressée
 * @param routine_tour La routine d'une tour
 * @param routine_unite La routine d'une unite
 */
void interface_main_client(client_t * client, int c, void *(*routine_tour)(void*), void *(*routine_unite)(void*));

#endif
