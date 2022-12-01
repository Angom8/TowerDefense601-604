#include "interface.h"

/**
 * Définition de la palette.
 */
void palette() {
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_RED);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_WHITE, COLOR_RED);
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);
    init_pair(7, COLOR_WHITE, COLOR_BLUE);
    init_pair(8, COLOR_BLACK, COLOR_BLUE); 
}

/**
 * Création d'une interface.
 * @param jeu l'état du jeu
 * @return l'interface créée
 */
interface_t interface_creer(jeu_t *jeu) {
    interface_t retour;
    int i, j;
  
    retour.outilsel = OUTIL_NONE;

    retour.infos = fenetre_creer(INFOS_POSX, INFOS_POSY, INFOS_LARGEUR, INFOS_HAUTEUR, "Informations", TRUE);
    wprintw(retour.infos->interieur, "Tapez 'Q' pour quitter");
    wprintw(retour.infos->interieur, "\n1, 2, 3 pour retirer des vies aux adversaires");
    wprintw(retour.infos->interieur, "\nV pour retirer des vies a l'utilisateur");
    wprintw(retour.infos->interieur, "\n$ pour ajouter de l'argent");
    wprintw(retour.infos->interieur, "\nF et U pour augmenter freeze et unfreeze");
    wrefresh(retour.infos->interieur);


    retour.outils = fenetre_creer(OUTILS_POSX, OUTILS_POSY, OUTILS_LARGEUR, OUTILS_HAUTEUR, "Outils", FALSE);
    interface_MAJOutils(&retour, jeu);

    retour.etat = fenetre_creer(ETAT_POSX, ETAT_POSY, ETAT_LARGEUR, ETAT_HAUTEUR, "Etat", FALSE);
    wprintw(retour.etat->interieur, "Vies      %2d\n", jeu->vies);
    wprintw(retour.etat->interieur, "Argent %5d\n", jeu->argent);
    wprintw(retour.etat->interieur, "Freeze     0\n");
    wprintw(retour.etat->interieur, "Unfreeze   0\n");
    wprintw(retour.etat->interieur, "Adv. 1    10\n");
    wprintw(retour.etat->interieur, "Adv. 2    10\n");
    wprintw(retour.etat->interieur, "Adv. 3    10\n");    
    wrefresh(retour.etat->interieur);

    retour.attaques = fenetre_creer(ATTAQUES_POSX, ATTAQUES_POSY, ATTAQUES_LARGEUR, ATTAQUES_HAUTEUR, "Attaques", FALSE);
    interface_MAJAttaques(&retour, jeu);
        
    retour.carte = fenetre_creer(CARTE_POSX, CARTE_POSY, CARTE_LARGEUR, CARTE_HAUTEUR, "Carte", FALSE);
    for(i = 0; i < 15; i++) {
        for(j = 0; j < 15; j++) {
            if(jeu->carte.cases[i][j] == CASE_VIDE) {
                waddch(retour.carte->interieur, ' ' | COLOR_PAIR(COULEUR_VIDE));
            }
            else if((jeu->carte.cases[i][j] >= CASE_MIN_JOUEUR) && (jeu->carte.cases[i][j] <= CASE_MAX_JOUEUR)) {
                wattron(retour.carte->interieur, COLOR_PAIR(COULEUR_JOUEUR));
                wprintw(retour.carte->interieur, "%d", jeu->carte.cases[i][j]);
            }
            else if(jeu->carte.cases[i][j] == CASE_ORDI) {
                wattron(retour.carte->interieur, COLOR_PAIR(COULEUR_ORDI));
                wprintw(retour.carte->interieur, "O");
            }
            else if(jeu->carte.cases[i][j] == CASE_FORT) {
                waddch(retour.carte->interieur, 'F' | COLOR_PAIR(COULEUR_FORT));
            }
            else if((jeu->carte.cases[i][j] >= CASE_MIN_CHEMIN) && (jeu->carte.cases[i][j] <= CASE_MAX_CHEMIN)) {
                waddch(retour.carte->interieur, ' ' | COLOR_PAIR(COULEUR_CHEMIN));
            }
        }
    }
    wrefresh(retour.carte->interieur);

    return retour;
}

/**
 * Suppression d'une interface.
 * @param interface l'interface à supprimer
 */
void interface_supprimer(interface_t *interface) {
    fenetre_supprimer(&interface->infos);
    fenetre_supprimer(&interface->outils);
    fenetre_supprimer(&interface->etat);
    fenetre_supprimer(&interface->carte);
    fenetre_supprimer(&interface->attaques);
}

/**
 * Met à jour la fenêtre 'Attaques'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJAttaques(interface_t *interface, jeu_t *jeu) {
    werase(interface->attaques->interieur);

    wattron(interface->attaques->interieur, COLOR_PAIR(1));
    if(jeu->argent < SOLDAT_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Soldat          %4d$ -> 123\n", SOLDAT_COUT);    
    
    if(jeu->argent < COMMANDO_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Commando        %4d$ -> 123\n", COMMANDO_COUT);
    
    if(jeu->argent < VEHICULE_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Vehicule blinde %4d$ -> 123\n", VEHICULE_COUT);
    
    if(jeu->argent < MISSILE_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Lance-missiles  %4d$ -> 123\n", MISSILE_COUT);

    if(jeu->argent < CHAR_COUT)
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Char            %4d$ -> 123\n", CHAR_COUT);

    if(jeu->freeze == 10)
        wattron(interface->attaques->interieur, COLOR_PAIR(1));
    else
        wattron(interface->attaques->interieur, COLOR_PAIR(4));
    wprintw(interface->attaques->interieur, "Freeze                -> 123\n");

    wrefresh(interface->attaques->interieur);
}

/**
 * Met à jour la fenêtre 'Outils'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJOutils(interface_t *interface, jeu_t *jeu) {
    werase(interface->outils->interieur);

    if(jeu->argent < TOUR_1_COUT) {
        wattron(interface->outils->interieur, COLOR_PAIR(4));
        if(interface->outilsel == OUTIL_TOUR_1)
            interface->outilsel = OUTIL_NONE;
    }
    else if(interface->outilsel == OUTIL_TOUR_1)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 1 %4d$\n", TOUR_1_COUT);

    if(jeu->argent < TOUR_2_COUT) {
        if(interface->outilsel == OUTIL_TOUR_2)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_2)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 2 %4d$\n", TOUR_2_COUT);

    if(jeu->argent < TOUR_3_COUT) {
        if(interface->outilsel == OUTIL_TOUR_3)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_3)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 3 %4d$\n", TOUR_3_COUT);

    if(jeu->argent < TOUR_4_COUT) {
        if(interface->outilsel == OUTIL_TOUR_4)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_4)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 4 %4d$\n", TOUR_4_COUT);

    if(jeu->argent < TOUR_5_COUT) {
        if(interface->outilsel == OUTIL_TOUR_5)
            interface->outilsel = OUTIL_NONE;
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    }
    else if(interface->outilsel == OUTIL_TOUR_5)
        wattron(interface->outils->interieur, COLOR_PAIR(5));
    else
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Tour 5 %4d$\n", TOUR_5_COUT);

    if(jeu->unfreeze < 10)
        wattron(interface->outils->interieur, COLOR_PAIR(4));
    else 
        wattron(interface->outils->interieur, COLOR_PAIR(1));
    wprintw(interface->outils->interieur, "Unfreeze\n");


    wrefresh(interface->outils->interieur);
}

/**
 * Met à jour la fenêtre 'Etat'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 */
void interface_MAJEtat(interface_t *interface, jeu_t *jeu) {
	mvwprintw(interface->etat->interieur, 0, 8, "%4d", jeu->vies);
	mvwprintw(interface->etat->interieur, 1, 7, "%5d", jeu->argent);
	mvwprintw(interface->etat->interieur, 2, 8, "%4d", jeu->freeze);
	mvwprintw(interface->etat->interieur, 3, 8, "%4d", jeu->unfreeze);
	mvwprintw(interface->etat->interieur, 4, 8, "%4d", jeu->adv[0]);
	mvwprintw(interface->etat->interieur, 5, 8, "%4d", jeu->adv[1]);
	mvwprintw(interface->etat->interieur, 6, 8, "%4d", jeu->adv[2]);
	
	wrefresh(interface->etat->interieur);
}

/**
 * Met à jour la fenêtre 'Carte'.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @return OK si réussite
 */
int interface_MAJCarte(interface_t *interface, jeu_t *jeu){
	size_t i, j;
	entite_t * entite;
	if(interface != NULL && jeu != NULL){
		for(i=0 ; i<CASES_MAX ; i++) {
			for(j=0 ; j<CASES_MAX ; j++) {
				entite = &jeu->entites[i][j];
				
				if(pthread_mutex_lock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Lock mutex\n");
					exit(EXIT_FAILURE);
				}
				
				if(entite->type == CASE_TOUR){
					
					if(pthread_mutex_lock(&jeu->mutex) == -1){
						fprintf(stderr, "Erreur : Lock mutex\n");
						exit(EXIT_FAILURE);
					}
					
					if(jeu->isFreeze == ON_FREEZE)
						mvwaddch(interface->carte->interieur, i, j, 'T' | COLOR_PAIR(COULEUR_TOUR_FREEZE));
					else
						mvwaddch(interface->carte->interieur, i, j, 'T' | COLOR_PAIR(COULEUR_VIDE));
					
					if(pthread_mutex_unlock(&jeu->mutex) == -1){
						fprintf(stderr, "Erreur : Unlock mutex\n");
						exit(EXIT_FAILURE);
					}
				}
				else if(entite->type == CASE_UNITE){
					switch(entite->unite.type){
						case UNITE_SOLDAT:
							mvwaddch(interface->carte->interieur, i, j, 'S' | COLOR_PAIR(COULEUR_CHEMIN));
							break;
						case UNITE_COMMANDO:
							mvwaddch(interface->carte->interieur, i, j, 'C' | COLOR_PAIR(COULEUR_CHEMIN));
							break;
						case UNITE_VEHICULE:
							mvwaddch(interface->carte->interieur, i, j, 'V' | COLOR_PAIR(COULEUR_CHEMIN));
							break;
						case UNITE_MISSILE:
							mvwaddch(interface->carte->interieur, i, j, 'M' | COLOR_PAIR(COULEUR_CHEMIN));
							break;
						case UNITE_CHAR:
							mvwaddch(interface->carte->interieur, i, j, 'H' | COLOR_PAIR(COULEUR_CHEMIN));
							break;
					}
				}
				else if(entite->type == CASE_VIDE){
					mvwaddch(interface->carte->interieur, i, j, ' ' | COLOR_PAIR(COULEUR_VIDE));
				}
				else if((entite->type >= CASE_MIN_CHEMIN) && (entite->type <= CASE_MAX_CHEMIN)){
					mvwaddch(interface->carte->interieur, i, j, ' ' | COLOR_PAIR(COULEUR_CHEMIN));
				}
		
				if(pthread_mutex_unlock(&entite->mutex) == -1){
					fprintf(stderr, "Erreur : Unlock mutex\n");
					exit(EXIT_FAILURE);
				}
				
			}
		}
		wrefresh(interface->carte->interieur);
		
		return EXIT_SUCCESS;
	}
	
	fprintf(stderr, "Erreur : L'interface ou le jeu est nul\n");
	exit(EXIT_FAILURE);
}

/**
 * Gestion des actions dans la fenêtre des outils.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre 
 */
void interface_outils(interface_t *interface, jeu_t *jeu, int posX, int posY) {
    switch(posY) {
        case 0:
            if(jeu->argent >= TOUR_1_COUT) {
                interface->outilsel = OUTIL_TOUR_1;
                wprintw(interface->infos->interieur, "\nOutil tour 1 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 1.");
        break;
        case 1:
            if(jeu->argent >= TOUR_2_COUT) {
                interface->outilsel = OUTIL_TOUR_2;
                wprintw(interface->infos->interieur, "\nOutil tour 2 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 2.");
        break;
        case 2:
            if(jeu->argent >= TOUR_3_COUT) {
                interface->outilsel = OUTIL_TOUR_3;
                wprintw(interface->infos->interieur, "\nOutil tour 3 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 3.");
        break;
        case 3:
            if(jeu->argent >= TOUR_4_COUT) {
                interface->outilsel = OUTIL_TOUR_4;
                wprintw(interface->infos->interieur, "\nOutil tour 4 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 4.");
        break;
        case 4:
            if(jeu->argent >= TOUR_5_COUT) {
                interface->outilsel = OUTIL_TOUR_5;                     
                wprintw(interface->infos->interieur, "\nOutil tour 5 selectionne. Cliquez sur un carre d'herbe.");
            }
            else
                wprintw(interface->infos->interieur, "\nPas assez d'argent pour acheter une tour 5.");
        break;
        case 5:
        	if(jeu->unfreeze >= VALUE_FREEZE_MAX){
                wprintw(interface->infos->interieur, "\nOutil unfreeze selectionne. Cliquez sur un carte pour le lancer.");
                interface->outilsel = OUTIL_UNFREEZE;
            }else{
                interface->outilsel = OUTIL_NONE;
                wprintw(interface->infos->interieur, "\nPas assez d'energie pour lancer un unfreeze.");
            }
        	break;
    }
}

/**
 * Gestion des actions dans la fenêtre des attaques.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 * @return Si réussite le numéro d'attaque
 */
int interface_attaques(interface_t *interface, jeu_t *jeu, int posX, int posY){
    int attaque = ATTAQUE_VIDE;
    if((posX >= 25) && (posX <= 27)) {
        switch(posY) {
            case 0:
                if(jeu->argent < SOLDAT_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un soldat.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= SOLDAT_COUT;
                    attaque = ATTAQUE_SOLDAT;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un soldat à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 1:
                if(jeu->argent < COMMANDO_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un commando.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= COMMANDO_COUT;
                    attaque = ATTAQUE_COMMANDO;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un commando à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 2:
                if(jeu->argent < VEHICULE_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un vehicule blinde.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= VEHICULE_COUT;
                    attaque = ATTAQUE_VEHICULE;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un vehicule blinde à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 3:
                if(jeu->argent < MISSILE_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un lance-missiles.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= MISSILE_COUT;
                    attaque = ATTAQUE_MISSILE;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un lance-missiles à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 4:
                if(jeu->argent < CHAR_COUT)
                    wprintw(interface->infos->interieur, "\nPas assez d'argent pour lancer un char.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->argent -= CHAR_COUT;
                    attaque = ATTAQUE_CHAR;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un char à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
            case 5:
                if(jeu->freeze < 10)
                    wprintw(interface->infos->interieur, "\nPas assez d'énergie pour lancer un freeze.");
                else if(jeu->adv[posX - 25] == 0)
                    wprintw(interface->infos->interieur, "\nL'adversaire %d est mort.", (posX - 24));
                else {
                    jeu->freeze = 0;
                    attaque = ATTAQUE_FREEZE;
                    wprintw(interface->infos->interieur, "\nEnvoi d'un freeze à l'adversaire %d... pour de faux", (posX - 24));
                }
                break;
        }
    }
    
    return attaque;
}

/**
 * Gestion des actions dans la fenêtre de la carte.
 * @param interface l'interface
 * @param jeu l'état du jeu
 * @param posX la position X du clic dans la fenêtre
 * @param posY la position Y du clic dans la fenêtre
 * @param routine_tour La routine d'une tour
 * @param routine_unite La routine d'une unite
 */
void interface_carte(interface_t *interface, jeu_t *jeu, int posX, int posY, void *(*routine_tour)(void*), void *(*routine_unite)(void*)){
    switch(interface->outilsel) {
        case OUTIL_NONE:
            /* Pas d'outils sélectionné : on affiche le contenu de la case */
            if(jeu->carte.cases[posY][posX] == CASE_VIDE) {
                wprintw(interface->infos->interieur, "\nOh !!! De l'herbe !!!");
            }
            else if((jeu->carte.cases[posY][posX] >= CASE_MIN_JOUEUR) && (jeu->carte.cases[posY][posX] <= CASE_MAX_JOUEUR)) {
                wprintw(interface->infos->interieur, "\nLe point de depart des unites de l'adversaire %d", (jeu->carte.cases[posY][posX] - CASE_MIN_JOUEUR + 1));
            }
            else if(jeu->carte.cases[posY][posX] == CASE_ORDI) {
                wprintw(interface->infos->interieur, "\nLe point de depart des vagues envoyees par l'ordinateur");
            }
            else if(jeu->carte.cases[posY][posX] == CASE_FORT) {
                wprintw(interface->infos->interieur, "\nLe fort a proteger");
            }
            else if((jeu->carte.cases[posY][posX] >= CASE_MIN_CHEMIN) && (jeu->carte.cases[posY][posX] <= CASE_MAX_CHEMIN)) {
                wprintw(interface->infos->interieur, "\nUne route...");
            }
            break;
        case OUTIL_TOUR_1:
      	if(ajouter_tour_jeu(jeu, OUTIL_TOUR_1, TOUR_1_COUT, posX, posY, routine_tour) == 0){
			wprintw(interface->infos->interieur, "\nTour 1 posee !");
      	}
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_2:
            if(ajouter_tour_jeu(jeu, OUTIL_TOUR_2, TOUR_2_COUT, posX, posY, routine_tour) == 0){
			wprintw(interface->infos->interieur, "\nTour 2 posee !");
      	}
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_3:
            if(ajouter_tour_jeu(jeu, OUTIL_TOUR_3, TOUR_3_COUT, posX, posY, routine_tour) == 0){
			wprintw(interface->infos->interieur, "\nTour 3 posee !");
      	}
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_4:
            if(ajouter_tour_jeu(jeu, OUTIL_TOUR_4, TOUR_4_COUT, posX, posY, routine_tour) == 0){
			wprintw(interface->infos->interieur, "\nTour 4 posee !");
      	}
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_TOUR_5:
           	if(ajouter_tour_jeu(jeu, OUTIL_TOUR_5, TOUR_5_COUT, posX, posY, routine_tour) == 0){
			wprintw(interface->infos->interieur, "\nTour 5 posee !");
      	}
            else {
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
        case OUTIL_UNFREEZE:
            if(lancer_unfreeze_jeu(jeu) == 0) {
                wprintw(interface->infos->interieur, "\nUNFREEEEEZE !!!");
            }
            else {
                interface->outilsel = OUTIL_NONE;
                wprintw(interface->infos->interieur, "\nDesole, pas possible...");
            }
            break;
    }
}
