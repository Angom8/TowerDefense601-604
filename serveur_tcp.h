#ifndef _SERVEUR_TCP_
#define _SERVEUR_TCP_

#define NB_CLIENTS 4
#define PORTS_MAX 65535

#define ETEINT 0
#define EN_ROUTE 1
#define EN_JEU 2

#include <time.h>
#include <pthread.h>

#include "adresse.h"
#include "socket.h"
#include "carte.h"
#include "scenario.h"
#include "jeu.h"

typedef struct{
	pthread_t thread;
	pthread_t thread_scenario;
	pthread_mutex_t mutex;
	socket_t socketserveur;
	adresse_t adresse;
	socket_t socketclients[NB_CLIENTS];
	unsigned int vies[NB_CLIENTS];
	size_t n_clients;
	unsigned short int etat;
	carte_t * carte;
	scenario_t * scenario;
	size_t n_scenario;
}serveur_tcp_t;

/**
 * Créer un serveur tcp 
 * @param serveur Le serveur tcp
 * @param carte La carte
 * @param scenario Le scenario
 * @param routine_serveur La routine du serveur
 * @return Le port si réussite
 */
int creer_serveur_tcp(serveur_tcp_t * serveur, carte_t * carte, scenario_t * scenario, void *(*routine_serveur)(void*));

/**
 * Attendre les clients du serveur tcp
 * @param serveur Le serveur
 * @return OK si réussite
 */
int attendre_clients_serveur_tcp(serveur_tcp_t * serveur);

/**
 * Envoyer des données aux clients du serveur tcp
 * @param serveur Le serveur
 * @param donnees Les données
 * @param taille La taille des données
 * @return OK si réussite
 */
int envoyer_donnees_clients_serveur_tcp(serveur_tcp_t * serveur, void * donnees, size_t taille);

/**
 * Envoyer aux autres clients qu'un client à perdu un point de vie
 * @param serveur Le serveur tcp
 * @param n Le numéro du client qui à perdu un point de vie
 * @return OK si réussite
 */
int envoyer_perdre_vie_clients_serveur_tcp(serveur_tcp_t * serveur, unsigned short int n);

/**
 * Lire le message des clients depuis un serveur tcp
 * @param serveur Le serveur tcp
 * @return OK si réussite
 */
int lire_message_clients_serveur_tcp(serveur_tcp_t * serveur);

/**
 * Tester si les 4 joueurs sont encore en vie depuis un serveur tcp
 * @param serveur Le serveur tcp
 * @return OK si au moins un joueur est encore en vie
 */
int test_joueurs_en_vie_serveur_tcp(serveur_tcp_t * serveur);

/**
 * Eteindre le serveur tcp en route
 * @param serveur Le serveur
 * @return OK si réussite
 */
int eteindre_serveur_tcp(serveur_tcp_t * serveur);

/**
 * Fermer le serveur tcp
 * @param serveur Le serveur
 * @return OK si réussite
 */
int fermer_serveur_tcp(serveur_tcp_t * serveur);

#endif
