#
# CONFIGURATION GENERALE
#

EXEC = serveur clients
OBJETS = nom.o adresse.o socket.o fichier.o message.o dossier.o fenetre.o interface.o ncurses.o coord.o carte.o scenario.o jeu.o entite.o serveur_tcp.o liste_serveurs_tcp.o tour.o unite.o client.o
NOM_PROJET = Serveur_Fichiers

#
# SUFFIXES
#

.SUFFIXES: .c .o

#
# OBJETS
#

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

#
# ARGUMENTS ET COMPILATEUR
#

CC = gcc
CCFLAGS = -Wall -O3 -ansi -pedantic -Werror -D _DEFAULT_SOURCE
CCLIBS = -lncurses -lpthread

#
# REGLES
#

all: $(OBJETS) $(EXEC_O)
	@echo "Creation des executables..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o $(OBJETS) $(CCLIBS); \
	done
	@echo "Termine."

#
# REGLES PAR DEFAUT
#

.c.o: .h
	@cd $(dir $<) && ${CC} ${CCFLAGS} -c $(notdir $<) -o $(notdir $@)

#
# REGLES GENERALES
#

clean:
	@echo "Suppresion des objets, des fichiers temporaires..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@rm -f dependances
	@echo "Termine."

depend:
	@echo "Creation des dependances..."
	@sed -e "/^# DEPENDANCES/,$$ d" makefile > dependances
	@echo "# DEPENDANCES" >> dependances
	@for i in $(OBJETS_O); do \
	$(CC) -MM -MT $$i $(CCFLAGS) `echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` >> dependances; \
	done
	@cat dependances > makefile
	@rm dependances
	@echo "Termine."

#
# CREATION ARCHIVE
#

ARCHIVE_FILES = *


archive: clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename "$$PWD"`; cd .. && tar zcf $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz') $(addprefix "$$REP"/,$(ARCHIVE_FILES))
	@echo "Termine."
	
zip:	clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.zip')..."
	@REP=`basename "$$PWD"`; cd .. && zip $(NOM_PROJET)$(shell date '+%y%m%d.zip') $(addprefix "$$REP"/,$(ARCHIVE_FILES))
	@echo "Termine."

# DEPENDANCES
nom.o: nom.c nom.h
adresse.o: adresse.c adresse.h
socket.o: socket.c socket.h adresse.h message.h dossier.h carte.h scenario.h
fichier.o: fichier.c fichier.h carte.h scenario.h
message.o: message.c message.h
dossier.o: dossier.c dossier.h fichier.h carte.h scenario.h
fenetre.o: fenetre.c fenetre.h
interface.o: interface.c interface.h
ncurses.o: ncurses.c ncurses.h
coord.o: coord.c coord.h
carte.o: carte.c carte.h nom.h fichier.h dossier.h coord.h
scenario.o: scenario.c scenario.h nom.h fichier.h dossier.h
jeu.o: jeu.c jeu.h
entite.o: entite.c entite.h
serveur_tcp.o: serveur_tcp.c serveur_tcp.h adresse.h socket.h carte.h scenario.h
liste_serveurs_tcp.o: liste_serveurs_tcp.c liste_serveurs_tcp.h
tour.o: tour.c tour.h
unite.o: unite.c unite.h
client.o: client.c client.h
serveur.o: serveur.c adresse.h socket.h dossier.h fichier.h carte.h scenario.h serveur_tcp.h liste_serveurs_tcp.h
clients.o: clients.c adresse.h socket.h dossier.h fichier.h carte.h scenario.h jeu.h tour.h unite.h client.h
