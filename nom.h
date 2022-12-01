#ifndef _NOM_
#define _NOM_

#define DESCRIPTION_MAX 256
#define NOM_MAX 64
#define FICHIERS_MAX 16

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

typedef struct nom_fichier_t{
	char nom[NOM_MAX];
}nom_fichier_t;

typedef struct{
	nom_fichier_t noms[FICHIERS_MAX];
	size_t n;
}liste_noms_fichiers_t;

/**
 * Afficher une liste de noms de fichiers
 * @param noms_fichiers La liste de noms de fichiers
 * @return OK si réussite
 */
int afficher_liste_noms_fichiers(liste_noms_fichiers_t * noms_fichiers);

#endif
