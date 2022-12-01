# tower_defence v
Projet 3 INFO0601 et INFO0604. Par ALVARO Luca (Code principal, Modélisation, Documentation) et THEBAULT Antoine (Corrections mineures, Documentation, Rapport)
  
##Lancer le jeu

* **Pour les clients :** ./clients adresseServeur portServeur
* **Pour le / les serveur(s)** ./serveur port

Exemple :
* /serveur 25565 (attention à votre serveur Minecraft !)
* /clients 127.1.0.0 25565

Protocole :
* Il faut lancer le serveur avant les clients.
* Fermer un client lors d'une partie risque de faire planter le serveur et les autres clients.

###Principal
* clients.c : Programme central pour l'execution du client. Comporte aussi la définition des threads pour certaines structures du jeu (client, entités, freeze/unfreeze).
* serveur.c : Programme central pour l'execution du serveur. Comporte aussi la définition des threads pour la création de sous-serveurs TCP (lançant des parties à partir de scénarios). On a aussi un semi-thread principal se chargeant des communications UDP pour récupérer les listes et agir en "première connexion".

###Communication
* adresse.c/h : Gestion de la création des adresses
* client.c/h : Gestion des actions du client et envoi haut-niveau de packets. (Créer une partie, rejoindre)
* listeserveurstcp.c/h : Gestion de la liste des serveurs de jeu (création, ajout) et de son envoi pour les clients la demandant.
* socket.c/h : Gestion de l'envoi et la réception de messages UDP ou TCP

###Lecture de fichiers
* carte.c/h : Gestion haut-niveau des cartes pour le jeu. Se base sur la lecture de fichiers via fichier.c/h
* dossier.c/h : Ajoute une couche pour permettre la gestion haut niveau des dossiers
* fichier.c/h : Permet (surtout la lecture) l'ouverture de fichiers en ajoutant une cache et éviter la surutilisation partout dans ke projet d'appels système. Comprend la lecture des cartes et scénarios.

###Jeu
* coord.c/h : Structure permettant la gestion de coordonnées x et y (ne contient qu'une seule fonction d'initialisation)
* entite.c/h : Structure regroupant les différentes composantes du jeu (soldat, fort, tour), invoquée à une coordonnée (x,y). Gère le multi-thread via pthread_mutex et cond. Ne comporte qu'une fonction d'initialisation.
* jeu.c/h : Structure permettant la gestion du jeu et de son déroulé (déplacement des unités, appels sur les threads composant la partie, initialisation, gestion de la carte)
* serveur_tcp.c/h : Sous-serveur, TCP, gérant les informations relatives à la partie en cours côté serveur (nombre de vies, clients, scénario en cours pour la partie). 1 serveur TCP = 1 partie
* tour.c/h : Gestion de la création des tours. Les constantes pour leurs caractéristiques peuvent être redéfinies ici. Pour simplifier, une approche statique avec 5 tours max a été privilégiée.
* unite.c/h : Gestion de la création des unités. Les constantes pour leurs caractéristiques peuvent être redéfinies ici.

###Affichage
* fenetre.c/h : Fichier fourni par M.RABAT sur gitlab pour l'affichage ncurses.
* interface.c/h : Fichier fourni par M.RABAT sur gitlab, puis largement adapté pour le projet avec l'ajout du support multi-threadé (ncurses n'étant pas thread-safe). Contient toute la partie intéraction avec l'interface de jeu et les appels sur les autres fonctions de communication avec le serveur.

