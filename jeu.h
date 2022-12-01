#ifndef _JEU_
#define _JEU_

#define VIES_JOUEURS 	10
#define N_ADV 		3
#define ARGENT 		150
#define UNITES_ATTENTES 100

/* Outils */
#define OUTIL_NONE       -1
#define OUTIL_TOUR_1      1
#define OUTIL_TOUR_2      2
#define OUTIL_TOUR_3      3
#define OUTIL_TOUR_4      4
#define OUTIL_TOUR_5      5
#define OUTIL_UNFREEZE    6

#define OFF_FREEZE 		0
#define ON_FREEZE			1
#define VALUE_FREEZE_MAX 	10

#include <errno.h>

#include "carte.h"
#include "entite.h"

/* La structure contenant les informations sur le jeu */
typedef struct {
	carte_t carte;					/* La carte */
	unsigned int vies;				/* Vies du joueur */
	unsigned int adv[3];				/* Vies des adversaires */
	unsigned int argent;				/* Argent */
	unsigned int freeze;				/* Etat freeze */
	unsigned int unfreeze;				/* Etat unfreeze */
	unsigned short int isFreeze;
	pthread_mutex_t mutex;
	pthread_cond_t cond_freeze;
	entite_t entites[CASES_MAX][CASES_MAX];
	entite_t unites[UNITES_ATTENTES];
}jeu_t;

/* Constantes pour les coûts des unités */
#define SOLDAT_COUT       100
#define COMMANDO_COUT     300
#define VEHICULE_COUT     700
#define MISSILE_COUT     1500
#define CHAR_COUT        3100

/* Constantes pour les coûts des tourelles */
#define TOUR_1_COUT       100
#define TOUR_2_COUT       300
#define TOUR_3_COUT       700
#define TOUR_4_COUT      1500
#define TOUR_5_COUT      3100

#define FREEZE_DUREE     5000

/**
 * Initialiser une partie
 * @param jeu Le jeu
 * @return OK si réussite
 */
int initialiser_jeu(jeu_t * jeu);

/**
 * Modifier l'argent d'un jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int modifier_argent_jeu(jeu_t * jeu, int argent);

/**
 * Incrementer la valeur de freeze et de unfreeze
 * @param jeu Le jeu
 * @return OK si réussite
 */
int incrementer_freeze_unfreeze(jeu_t * jeu);

/**
 * Retirer une vie du joueur
 * @param jeu Le jeu
 * @return OK si réussite
 */
int retirer_vie_joueur_jeu(jeu_t * jeu);

/**
 * Retirer une vie du joueur
 * @param jeu Le jeu
 * id L'id de l'adversaire
 * @return OK si réussite
 */
int retirer_vie_adversaires_jeu(jeu_t * jeu, unsigned short int id);

/**
 * Ajouter une tour dans un jeu
 * @param jeu Le jeu
 * @param type Le type de tour
 * @param argent L'argent à payer
 * @param posX La position x de la tour
 * @param posY La position y de la tour
 * @param routine_tour La routine d'une tour
 * @param OK si réussite
 */
int ajouter_tour_jeu(jeu_t * jeu, long type, int argent, int posX, int posY, void *(*routine_tour)(void*));

/**
 * Ajouter une unité dans un jeu
 * @param jeu Le jeu
 * @param type Le type d'unité
 * @param posX La position x de l'unité
 * @param posY La position y de l'unité
 * @param routine_unite La routine d'une unite
 * @param OK si réussite
 */
int ajouter_unite_jeu(jeu_t * jeu, long type, int posX, int posY, void *(*routine_unite)(void*));

/**
 * Tirer sur une unité depuis une tour
 * @param jeu Le jeu
 * @param entite La tour
 * @return OK si réussite
 */
int tirer_tour_jeu(jeu_t * jeu, entite_t * entite);

/**
 * Déplacer une unité sur le chemin
 * @param jeu Le jeu
 * @param entite L'unité
 * @return Le pointeur de la nouvelle entité si réussite
 */
entite_t * deplacer_unite_jeu(jeu_t * jeu, entite_t * entite, unsigned short int * fin);

/**
 * Attendre sur une variable de condition le temps du freeze pour une tour
 * @param jeu Le jeu
 * @return OK si réussite
 */
int attendre_freeze_tour_jeu(jeu_t * jeu);

/**
 * Lancer un unfreeze des tours dans le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int lancer_unfreeze_jeu(jeu_t * jeu);

/**
 * Lancer un freeze des tours dans le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int lancer_freeze_jeu(jeu_t * jeu);

/**
 * Test si le joueur est toujours en vie
 * @param jeu Le jeu
 * @return OK si le joueur est toujours en vie
 */
int test_en_vie_jeu(jeu_t * jeu);

/**
 * Fermer le jeu
 * @param jeu Le jeu
 * @return OK si réussite
 */
int fermer_jeu(jeu_t * jeu);

#endif
