#ifndef _FICHIER_
#define _FICHIER_

#define NOM_MAX 64
#define FICHIERS_MAX 16

#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include "scenario.h"
#include "carte.h"

typedef struct{
	int id;
}fichier_t;

/**
 * Ouvrir un fichier en lecture seulement
 * @param nom_fichier Le nom du fichier
 * @return Le fichier si réussite
 */
fichier_t ouvrir_fichier_lecture(char * nom_fichier);

/**
 * Retourner la taille d'un fichier
 * @param fichier Le fichier
 * @return La taille du fichier si réussite
 */
off_t recuperer_taille_fichier(fichier_t * fichier);

/**
 * Lire la description d'un fichier
 * @param fichier Le fichier
 * @param description La description à initialiser
 * @return OK si réussite
 */
int lire_fichier_description(fichier_t * fichier, char * description);

/**
 * Lire un unsigned char dans un fichier
 * @param fichier Le fichier
 * @param uc Le unsigned char à initialiser
 * @param OK si réussite
 */
int lire_fichier_unsigned_char(fichier_t * fichier, unsigned char * uc);

/**
 * Lire une entree dans un fichier
 * @param fichier Le fichier
 * @param entree L'entrée à initialiser
 * @return OK si réussite
 */
int lire_fichier_entree(fichier_t * fichier, entree_t * entree);

/**
 * Lire une carte dans un fichier
 * @param carte La carte à initialiser avec les données du fichier
 * @param nom_fichier Le nom du fichier
 * @return OK si réussite
 */
int lire_carte(carte_t * carte, char * nom_fichier);

/**
 * Lire un scénario dans un fichier
 * @param scenario Le scénario à initialiser avec les données du fichier
 * @param nom_fichier Le nom du fichier
 * @return OK si réussite
 */
int lire_scenario(scenario_t * scenario, char * nom_fichier);

/**
 * Ouvrir un fichier en lecture seulement
 * @param fichier Le fichier
 * @return OK si réussite
 */
int fermer_fichier(fichier_t * fichier);

#endif
