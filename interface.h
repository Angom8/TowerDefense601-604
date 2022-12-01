#ifndef _INTERFACE_
#define _INTERFACE_

/* Propriétés de la fenêtre d'informations */
#define INFOS_POSX        0 /* Position en X */
#define INFOS_POSY        0 /* Position en Y */
#define INFOS_LARGEUR    60 /* Largeur avec le cadre */
#define INFOS_HAUTEUR    10 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre de la carte */
#define CARTE_POSX        5 /* Position en X */
#define CARTE_POSY       10 /* Position en Y */
#define CARTE_LARGEUR    17 /* Largeur avec le cadre */
#define CARTE_HAUTEUR    17 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des outils */
#define OUTILS_POSX      23 /* Position en X */
#define OUTILS_POSY      10 /* Position en Y */
#define OUTILS_LARGEUR   15 /* Largeur avec le cadre */
#define OUTILS_HAUTEUR    9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre d'état */
#define ETAT_POSX        39 /* Position en X */
#define ETAT_POSY        10 /* Position en Y */
#define ETAT_LARGEUR     15 /* Largeur avec le cadre */
#define ETAT_HAUTEUR      9 /* Hauteur avec le cadre */

/* Propriétés de la fenêtre des attaques */
#define ATTAQUES_POSX    23 /* Position en X */
#define ATTAQUES_POSY    19 /* Position en Y */
#define ATTAQUES_LARGEUR 31 /* Largeur avec le cadre */
#define ATTAQUES_HAUTEUR  8 /* Hauteur avec le cadre */

/* Dimensions de l'interface (pour vérification avec celles du terminal) */
#define LARGEUR INFOS_LARGEUR
#define HAUTEUR ATTAQUES_POSY + ATTAQUES_HAUTEUR

/* Couleurs des cases */
#define COULEUR_VIDE      	2
#define COULEUR_FORT      	7
#define COULEUR_CHEMIN    	3
#define COULEUR_JOUEUR    	6
#define COULEUR_ORDI      	6
#define COULEUR_TOUR_FREEZE	8

#define ATTAQUE_VIDE		0
#define ATTAQUE_SOLDAT 		1
#define ATTAQUE_COMMANDO 	2
#define ATTAQUE_VEHICULE	3
#define ATTAQUE_MISSILE		4
#define ATTAQUE_CHAR		5
#define ATTAQUE_FREEZE		6

#include <unistd.h>
#include <fcntl.h>

#include "fenetre.h"
#include "ncurses.h"
#include "jeu.h"
#include "socket.h"

/* Structure représentant l'interface de l'application */
typedef struct {
    fenetre_t *infos;    /* Fenêtre d'informations */
    fenetre_t *outils;   /* Fenêtre des outils */
    fenetre_t *carte;    /* Fenêtre de la carte */
    fenetre_t *etat;     /* Fenêtre d'état */
    fenetre_t *attaques; /* Fenêtre des attaques */
    unsigned int outilsel; /* L'outil sélectionné */
}interface_t;

/**
 * Définition de la palette.
 */
void palette();

/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
interface_t interface_creer(jeu_t *jeu);

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(interface_t *interface);

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(interface_t *interface, jeu_t *jeu);

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(interface_t *interface, jeu_t *jeu);

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(interface_t *interface, jeu_t *jeu);

/**
 * Met à jour la fenêtre 'Carte'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @return OK si réussite
 */
int interface_MAJCarte(interface_t *interface, jeu_t *jeu);

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_outils(interface_t *interface, jeu_t *jeu, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 * @return Si réussite le numéro d'attaque
 */
int interface_attaques(interface_t *interface, jeu_t *jeu, int posX, int posY);

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 * @param routine_tour La routine d'une tour
 * @param routine_unite La routine d'une unite
 */
void interface_carte(interface_t *interface, jeu_t *jeu, int posX, int posY, void *(*routine_tour)(void*), void *(*routine_unite)(void*));

#endif
