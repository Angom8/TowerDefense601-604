/**
 * Ce programme est le serveur udp qui met en relation les clients et qui crée les serveurs tcp
 * @author Luca Alvaro
 * @author Antoine Thebault
 **/

#define _POSIX_C_SOURCE 200112L

#include <stdlib.h>      /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>       /* Pour printf, fprintf, perror */
#include <sys/socket.h>  /* Pour socket, bind */
#include <arpa/inet.h>   /* Pour sockaddr_in */
#include <string.h>      /* Pour memset */
#include <unistd.h>      /* Pour close */
#include <errno.h>       /* Pour errno */
#include <signal.h>

#include "adresse.h"
#include "socket.h"
#include "fichier.h"
#include "message.h"
#include "dossier.h"
#include "carte.h"
#include "scenario.h"
#include "serveur_tcp.h"
#include "liste_serveurs_tcp.h"

int quitter = 0;

void handler_serveur(int signum){
	if(signum == SIGINT){
		quitter = 1;
	}
}

/**
 * Routine de la gestion du scénario d'un serveur tcp
 * @param arg Le pointeur sur les données du serveur tcp
 */
void *routine_scenario_serveur_tcp(void * arg){
	message_t message;
	struct timespec time;
	serveur_tcp_t * serveur = (serveur_tcp_t*)arg;
	scenario_t * scenario = serveur->scenario;
	
	while(serveur->n_scenario < scenario->n_entrees){
		time.tv_sec = scenario->entrees[serveur->n_scenario].temps/1000;
		time.tv_nsec = (scenario->entrees[serveur->n_scenario].temps%1000)*1000000;
		nanosleep(&time, NULL);
		
		/* Initialisation du message */
		if(scenario->entrees[serveur->n_scenario].type == TYPE_UNITE){
			initialiser_message_3_entiers(&message, TYPE_ENVOI_ENTREES, scenario->entrees[serveur->n_scenario].type, scenario->entrees[serveur->n_scenario].donnee, CASE_ORDI);
		}
		else{
			initialiser_message_2_entiers(&message, TYPE_ENVOI_ENTREES, scenario->entrees[serveur->n_scenario].type, scenario->entrees[serveur->n_scenario].donnee);
		}
		
		/* Envoi du message */
		if(pthread_mutex_lock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		envoyer_donnees_clients_serveur_tcp(serveur, &message, sizeof(message_t));
		
		if(pthread_mutex_unlock(&serveur->mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		
		/* Envoi des donnees du buffer si besoin */
		if(scenario->entrees[serveur->n_scenario].type == TYPE_MESSAGE){
			if(pthread_mutex_lock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Lock mutex\n");
				exit(EXIT_FAILURE);
			}
			
			envoyer_donnees_clients_serveur_tcp(serveur, scenario->entrees[serveur->n_scenario].donnees, sizeof(scenario->entrees[serveur->n_scenario].donnees));
			
			if(pthread_mutex_unlock(&serveur->mutex) == -1){
				fprintf(stderr, "Erreur : Unlock mutex\n");
				exit(EXIT_FAILURE);
			}
		}
		
		serveur->n_scenario++;
	}
	
	return NULL;
}

/**
 * Routine d'un serveur tcp
 * @param arg Le pointeur sur les données du serveur tcp
 */
void *routine_serveur_tcp(void * arg){
	struct timespec time;
	serveur_tcp_t * serveur = (serveur_tcp_t*)arg;
	
	time.tv_sec = 0;
	time.tv_nsec = 10*1000000;
	
	attendre_clients_serveur_tcp(serveur);
	
	if(pthread_create(&serveur->thread_scenario, NULL, routine_scenario_serveur_tcp, (void *)serveur) == -1){
		fprintf(stderr, "Erreur : Creation de thread\n");
		exit(EXIT_FAILURE);
	}
	
	while(test_joueurs_en_vie_serveur_tcp(serveur) == 0){
		/* Routine du serveur */
		nanosleep(&time, NULL);
		lire_message_clients_serveur_tcp(serveur);
	}
	
	eteindre_serveur_tcp(serveur);
	
	return NULL;
}

/**
 * Routine du serveur udp
 * @param socket La socket
 * @param cartes La liste de cartes
 * @param scenarios La liste de scénarios
 * @param serveurs La liste de serveurs tcp
 * @return OK si réussite
 */
int routine_serveur_udp(socket_t * socket, liste_cartes_t * cartes, liste_scenarios_t * scenarios, liste_serveurs_tcp_t * serveurs){
	adresse_t adresse_client;
	message_t message;
	int port = 0;
	
	if(socket != NULL && cartes != NULL && scenarios != NULL && serveurs != NULL){
		
		adresse_client = recevoir_donnees_socket_udp(socket, &message, sizeof(message_t));
		
		switch(message.type){
			case TYPE_DEMANDE_LISTE_CARTES:
				/* Envoi de la listes des noms des cartes */
				envoyer_liste_noms_cartes_udp(socket, &adresse_client, cartes);
				printf("Serveur : liste de cartes envoyée.\n");
				break;
			
			case TYPE_DEMANDE_LISTE_SCENARIOS:
				/* Envoi de la listes des noms des scenarios */
				envoyer_liste_noms_scenarios_udp(socket, &adresse_client, scenarios);
				printf("Serveur : liste de scenarios envoyée.\n");
				break;
			
			case TYPE_DEMANDE_LISTE_SERVEURS:
				/* Envoi de la listes des noms des serveurs */
				envoyer_liste_noms_serveurs_udp(socket, &adresse_client, serveurs);
				printf("Serveur : liste de serveurs envoyée.\n");
				break;
				
			case TYPE_CHOIX_NOUVELLE_PARTIE:
				/* Création du serveur tcp si les données sont ok */
				if(message.i > 0 && message.j > 0 && message.i <= cartes->n && message.j <= scenarios->n){
					port = ajouter_serveur_tcp_liste_serveurs_tcp(serveurs, &cartes->cartes[message.i-1], 
													   &scenarios->scenarios[message.j-1], routine_serveur_tcp);
					printf("Serveur : création du serveur tcp.\n");
				}
				
				/* Envoie du port du serveur tcp (si port == 0 : erreur) */
				initialiser_message_entier(&message, TYPE_ENVOI_PORT, port);
				envoyer_donnees_socket_udp(socket, &adresse_client, &message, sizeof(message_t));
				printf("Serveur : port envoyé.\n");
				break;
				
			case TYPE_CHOIX_REJOINDRE_PARTIE:
				if(!recuperer_port_serveur_tcp(serveurs, &port, message.i-1)){port = 0;}
				
				/* Envoi du port du serveur tcp */
				initialiser_message_entier(&message, TYPE_ENVOI_PORT, port);
				envoyer_donnees_socket_udp(socket, &adresse_client, &message, sizeof(message_t));
				printf("Serveur : port récupéré et envoyé.\n");
				break;
		}
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : La socket ou la liste de cartes ou scenarios ou serveurs est nulle\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	struct sigaction action;
	liste_serveurs_tcp_t serveurs;
	liste_cartes_t cartes;
	liste_scenarios_t scenarios;
	socket_t socket;
	adresse_t adresseServeur;

	/* Récupération des arguments */
	if(argc != 2) {
		fprintf(stderr, "Usage : %s port\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "  port  : port d'écoute du serveur\n");
		exit(EXIT_FAILURE);
	}
	
	/* Initialisation de action */
	action.sa_handler = handler_serveur;
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	if(sigaction(SIGINT, &action, NULL) == -1) {
		perror("Erreur lors du positionnement ");
		exit(EXIT_FAILURE);
	}
	
	/* Initialiser la liste de cartes du dossier cartes */
	generer_liste_cartes(&cartes);
	
	/* Initialiser la liste de scenarios du dossier scenarios */
	generer_liste_scenarios(&scenarios);

	/* Création de la socket */
	creer_socket_udp(&socket);

	/* Création de l'adresse du serveur */
	creer_adresse_entier(&adresseServeur, htons(atoi(argv[1])), htonl(INADDR_ANY));

	/* Nommage de la socket */
	nommage_socket(&socket, &adresseServeur);
	
	printf("CTRL + C pour fermer le serveur\n");
	/* Attente de la requête d'un client */
	while(quitter == 0){
		/* Routine du serveur udp */
		routine_serveur_udp(&socket, &cartes, &scenarios, &serveurs);
	}
	
	/* Fermeture des serveurs tcp */
	fermer_liste_serveurs_tcp(&serveurs);

	/* Fermeture de la socket */
	fermer_socket(&socket);

	printf("\nServeur terminé.\n");

	return EXIT_SUCCESS;
}
