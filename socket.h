#ifndef _SOCKET_
#define _SOCKET_

#define TAILLE_MAX 256

#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>

#include "adresse.h"
#include "message.h"
#include "dossier.h"
#include "carte.h"
#include "scenario.h"

typedef struct{
	int socketfd;
}socket_t;

/**
 * Créer une socket en mode udp
 * @param socket La socket
 * @return OK si réussite
 */
int creer_socket_udp(socket_t * s);

/**
 * Créer une socket en mode tcp
 * @param socket La socket
 * @return OK si réussite
 */
int creer_socket_tcp(socket_t * s);

/**
 * Nommer la socket
 * @param socket La socket
 * @param adresse L'adresse
 * @return OK si réussite
 */
int nommage_socket(socket_t * socket, adresse_t * adresse);

/**
 * Mettre en mode passif
 * @param socket La socket
 * @return OK si réussite
 */
int mode_passif_socket(socket_t * socket);

/**
 * Connexion au serveur
 * @param socket La socket
 * @param adresse L'adresse
 * @return OK si réussite
 */
int connexion_socket(socket_t * socket, adresse_t * adresse);

/**
 * Attendre une connexion
 * @param socket La socket
 * @return La socket du client si réussite
 */
socket_t attendre_connexion_socket_client(socket_t * socket);

/**
 * Envoyer des données avec une socket en mode udp
 * @param socket La socket
 * @param adresse L'adresse
 * @param donnees Les données à envoyer
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_socket_udp(socket_t * socket, adresse_t * adresse, void * donnees, size_t taille);

/**
 * Envoyer une liste des noms de cartes avec une socket
 * @param socket La socket
 * @param adresse L'adresse
 * @param cartes La liste de cartes
 * @return OK si réussite
 */
int envoyer_liste_noms_cartes_udp(socket_t * socket, adresse_t * adresse, liste_cartes_t * cartes);

/**
 * Envoyer une liste des noms de scenarios avec une socket
 * @param socket La socket
 * @param adresse L'adresse
 * @param scenarios La liste de scenarios
 * @return OK si réussite
 */
int envoyer_liste_noms_scenarios_udp(socket_t * socket, adresse_t * adresse, liste_scenarios_t * scenarios);

/**
 * Envoyer des données avec une socket en mode tcp
 * @param socket La socket
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_socket_tcp(socket_t * socket, void * donnees, size_t taille);

/**
 * Recevoir des données depuis une socket en mode udp
 * @param socket La socket
 * @param donnees Les données à recevoir
 * @param taille La taille des données
 * @return L'adresse de l'envoyeur
 */
adresse_t recevoir_donnees_socket_udp(socket_t * socket, void * donnees, size_t taille);

/**
 * Recevoir des données depuis une socket client en mode tcp
 * @param socket La socket client
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int recevoir_donnees_socket_tcp(socket_t * socket, void * donnees, size_t taille);

/**
 * Recevoir des données en mode non bloquant depuis une socket client en mode tcp
 * @param socket La socket client
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int recevoir_donnees_non_bloquant_socket_tcp(socket_t * socket, void * donnees, size_t taille);

/**
 * Fermer une socket
 * @param socket La socket
 * @return OK si réussite
 */
int fermer_socket(socket_t * socket);

#endif
