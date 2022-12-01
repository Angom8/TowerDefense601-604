#ifndef _LISTE_SERVEURS_TCP_
#define _LISTE_SERVEURS_TCP_

#define NB_SERVEURS_TCP 10

#include "fichier.h"
#include "serveur_tcp.h"

typedef struct{
	serveur_tcp_t serveurs[NB_SERVEURS_TCP];
}liste_serveurs_tcp_t;

/**
 * Initialiser une liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @return OK si réussite
 */
int initialiser_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs);

/**
 * Ajouter un serveur à la liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @param carte La carte
 * @param scenario Le scenario
 * @param routine_serveur La routine du serveur
 * @return Le port si réussite
 */
int ajouter_serveur_tcp_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs, carte_t * carte, scenario_t * scenario, void *(*routine_serveur)(void*));

/**
 * Générer une liste de nom de fichiers à partir d'une liste de serveurs tcp
 * @param serveurs La liste de serveurs
 * @param fichiers La liste de noms de fichiers à générer
 * @return OK si réussite
 */
int generer_liste_noms_fichiers_serveurs(liste_serveurs_tcp_t * serveurs, liste_noms_fichiers_t * fichiers);

/**
 * Envoyer une liste des noms des serveurs tcp avec une socket en mode udp
 * @param socket La socket
 * @param adresse L'adresse
 * @param serveurs La liste de serveurs
 * @return OK si réussite
 */
int envoyer_liste_noms_serveurs_udp(socket_t * socket, adresse_t * adresse, liste_serveurs_tcp_t * serveurs);

/**
 * Récupérer le port d'un serveur tcp depuis une liste de serveurs tcp
 * @param serveurs La liste de serveurs
 * @param port Le port à récupérer
 * @param n Le numéro du serveur tcp
 * @return OK si réussite
 */
int recuperer_port_serveur_tcp(liste_serveurs_tcp_t * serveurs, int * port, int n);

/**
 * Fermer tous les serveurs en marche d'une liste de serveurs tcp
 * @param serveurs La liste de serveurs tcp
 * @return OK si réussite
 */
int fermer_liste_serveurs_tcp(liste_serveurs_tcp_t * serveurs);

#endif
