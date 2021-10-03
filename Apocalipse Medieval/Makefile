ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: ApocalipseMedieval.exe funcoesEProcessos.o

ApocalipseMedieval.exe: ApocalipseMedieval.o funcoesEProcessos.o
	gcc -IModulos -o ApocalipseMedieval.exe ApocalipseMedieval.o Modulos/funcoesEProcessos.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

ApocalipseMedieval.o: ApocalipseMedieval.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c ApocalipseMedieval.c

funcoesEProcessos.o:
	gcc -IModulos -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c Modulos/funcoesEProcessos.c 
	move "funcoesEProcessos.o" "Modulos"
clean:
	del ApocalipseMedieval.o
	del ApocalipseMedieval.exe
	cd Modulos && del funcoesEProcessos.o

	
	
	

