/**
 * Ce programme est le client.
 * L'adresse IP et le port du serveur sont passés en argument du programme.
 * @author Luca Alvaro
 * @author Antoine Thebault
 **/

#include <stdlib.h>      /* Pour exit, EXIT_FAILURE, EXIT_SUCCESS */
#include <stdio.h>       /* Pour printf, fprintf, perror */
#include <sys/socket.h>  /* Pour socket */
#include <arpa/inet.h>   /* Pour sockaddr_in, inet_pton */
#include <string.h>      /* Pour memset */
#include <unistd.h>      /* Pour close */
#include <errno.h>       /* Pour errno */

#include "adresse.h"
#include "socket.h"
#include "fichier.h"
#include "message.h"
#include "dossier.h"
#include "carte.h"
#include "scenario.h"
#include "ncurses.h"
#include "fenetre.h"
#include "jeu.h"
#include "interface.h"
#include "serveur_tcp.h"
#include "tour.h"
#include "unite.h"
#include "client.h"

client_t client;

/* ------------------------------------------------------ Gestion multi-thrad ------------------------------------ */

/**
* Thread chargé de la mise à jour de l'interface client lors d'une partie.
* @param fichier de structure de gestion du client en cours
* @return le client prend fin si le thread s'arrete
*/
void *routine_affichage_client(void *arg){
	struct timespec time;
	client_t * c = (client_t*)arg;
	
	time.tv_sec = 0;
	time.tv_nsec = 100*1000000;
	
	while(test_en_vie_jeu(&c->jeu) == 0){
		nanosleep(&time, NULL);
		
		interface_MAJCarte(&c->interface, &c->jeu);
		
		if(pthread_mutex_lock(&c->jeu.mutex) == -1){
			fprintf(stderr, "Erreur : Lock mutex\n");
			exit(EXIT_FAILURE);
		}
		interface_MAJAttaques(&c->interface, &c->jeu);
		interface_MAJOutils(&c->interface, &c->jeu);
		interface_MAJEtat(&c->interface, &c->jeu);
		if(pthread_mutex_unlock(&c->jeu.mutex) == -1){
			fprintf(stderr, "Erreur : Unlock mutex\n");
			exit(EXIT_FAILURE);
		}
		wrefresh(c->interface.infos->interieur);
	}
	
	return NULL;
}

/**
* Thread chargé de l'incrémentation de la charge pour la capacité de freeze/unfreeze.
* @param fichier de structure de gestion du jeu en cours
* @return le client prend fin si le thread s'arrete
*/
void *routine_incrementer_freeze_unfreeze(void *arg){
	jeu_t * jeu = (jeu_t*)arg;
	
	while(test_en_vie_jeu(&client.jeu) == 0){
		sleep(3);
		incrementer_freeze_unfreeze(jeu);
	}
	
	return NULL;
}

/**
* Thread chargé de la gestion individuelle pour chacune des tours
* @param fichier de structure de gestion du jeu en cours
* @return le client prend fin si le thread s'arrete
*/
void *routine_tour_client(void * arg){
	struct timespec time;
	entite_t * entite = (entite_t*)arg;
	
	time.tv_sec = entite->tour.vitesse/1000;
	time.tv_nsec = (entite->tour.vitesse%1000)*1000000;
	
	while(test_en_vie_jeu(&client.jeu) == 0){
		/* Freeze la tour si la variable 'isFreeze' est active */
		attendre_freeze_tour_jeu(&client.jeu);
		
		tirer_tour_jeu(&client.jeu, entite);
		
		nanosleep(&time, NULL);
	}
	
	return NULL;
}

/**
* Thread chargé de la gestion individuelle pour chacune des tours
* @param fichier de structure de gestion de l'entité en cours
* @return null
*/
void *routine_unite_client(void *arg){
	struct timespec time;
	unsigned short int fin = 0;
	entite_t * entite = (entite_t*)arg;
	
	time.tv_sec = entite->unite.vitesse/1000;
	time.tv_nsec = (entite->unite.vitesse%1000)*1000000;
	
	time.tv_sec = 1;
	time.tv_nsec = 0;
	
	/* Type d'annulation */
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
	
	while(fin == 0 && test_en_vie_jeu(&client.jeu) == 0){
		nanosleep(&time, NULL);
		entite = deplacer_unite_jeu(&client.jeu, entite, &fin);
	}
	
	if(fin == 1){
		/* Retire un point de vie */
		if(retirer_vie_joueur_jeu(&client.jeu) == 0){
			/* Envoyer la perte d'un point de vie */
			envoyer_perdre_vie_client(&client);
		}
	}
	
	return NULL;
}


/* -------------------------------------------------------- Client propre -------------------------------- */

int main(int argc, char *argv[]) {
	int ch;
  	bool quitter = FALSE;
  
	/* Vérification des arguments */
	if(argc != 3) {
		fprintf(stderr, "Usage : %s adresseServeur portServeur\n", argv[0]);
		fprintf(stderr, "Où :\n");
		fprintf(stderr, "  adresseServeur : adresse IPv4 du serveur\n");
		fprintf(stderr, "  portServeur    : numéro de port du serveur\n");
		exit(EXIT_FAILURE);
	}
	
	/* Initialisation du client */
	initialiser_client(&client, htons(atoi(argv[2])), argv[1]);
	
	printf("~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("-----------------------\n");
	printf("-----Tower Defense-----\n");
	printf("-----------------------\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~\n");

	printf("\n> Bienvenue sur le menu du jeu ! <\nᕦ(ò_ó)ᕤ Prouvez votre valeur avec 3 autres joueurs ! ᕦ(ò_ó)ᕤ\n\n");
	
	/* Choisir une action */
	choisir_action_client(&client);
	
	/* Connexion au serveur tcp */
	connexion_serveur_tcp_client(&client);
	
	/* Réception de la carte en mode tcp */
	recevoir_donnees_socket_tcp(&client.socket, &client.jeu.carte, sizeof(carte_t));
	
	/* Initialisation du jeu */
	initialiser_jeu(&client.jeu);
	
	/* Attendre les autres clients */
	attendre_adversaire_client(&client);
	
	/* Initialisation de ncurses */
	ncurses_initialiser();
	ncurses_souris();
	ncurses_couleurs(); 
	palette();
	clear();
	refresh();
	
	/* Vérification des dimensions du terminal */
	if((COLS < LARGEUR) || (LINES < HAUTEUR)) {
		ncurses_stopper();
		fprintf(stderr, "Les dimensions du terminal sont insufisantes : l=%d,h=%d au lieu de l=%d,h=%d\n", COLS, LINES, LARGEUR, HAUTEUR);
		exit(EXIT_FAILURE);
	}
	
	/* Création de l'interface */
	client.interface = interface_creer(&client.jeu);
	
	/* Création du thread qui permet d'incrémenter la valeur du freeze et unfreeze toutes les 3 secondes */
	if(pthread_create(&client.thread_incrementer_freeze_unfreeze, NULL, routine_incrementer_freeze_unfreeze, (void *)&client.jeu) == -1){
		fprintf(stderr, "Erreur : Creation de thread\n");
		exit(EXIT_FAILURE);
	}
	
	/* Création du thread qui permet la mise à jour de l'affichage */
	if(pthread_create(&client.thread_affichage, NULL, routine_affichage_client, (void *)&client) == -1){
		fprintf(stderr, "Erreur : Creation de thread\n");
		exit(EXIT_FAILURE);
	}

	/* Boucle principale */
	while(quitter == FALSE) {
		timeout(100);
		ch = getch();
		if((ch == 'Q') || (ch == 'q'))
			quitter = true;
		else
			interface_main_client(&client, ch, routine_tour_client, routine_unite_client);
	}
	
	/* Envoyer au serveur la fermeture du client */
	envoyer_fin_client(&client);
	
	/* Fermer le client */
	fermer_client(&client);

	/* Arrêt de ncurses */
	ncurses_stopper();

	return EXIT_SUCCESS;
}
