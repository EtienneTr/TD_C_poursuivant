#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "obstacles.h"

#define TAILLE_MAX 256

int initObstacle(){
	FILE* file = NULL;
	char Chaine[TAILLE_MAX] = "";
	char *chaineX, *chaineY;
	int tab[256];
	int i = 0;
	file = fopen("obstacles.txt", "r");
	if(file != NULL){
		while(fgets(Chaine, TAILLE_MAX, file) != NULL){
			//printf("Chaine lue : %s\n", Chaine);
			//on évite les commentaires
			if(Chaine[0] == '#' || Chaine[0] == ' ') {
				continue;
			} else {
				//printf("test");
				chaineX = strtok(Chaine, ";");
				chaineY = strtok(NULL, ";");
				tab[i] = atoi(chaineX);
				tab[i+1] = atoi(chaineY);
				i += 2;
				//fprintf(stdout, "x %d\n", i);
			}
		}
		fclose(file);
		fprintf(stdout, "x %d\n", i);
		for(int y=0; y<i; y++){
			fprintf(stdout, "%d", tab[y]);
		}
		ajoutObstacle(tab, i);
		
	}else{
		printf("Ouverture impossible");
		return 0;
	}
	return 1;
}

//Ajoute l'obstacle à l'interface
void ajoutObstacle(int tabObs[256], int lg){
	int x, y;
	for(int i=0; i<lg; i+=2){
		x = tabObs[i];
		y = tabObs[i+1];
		if(x < TAILLE_VE && y < TAILLE_HO){
			tab[x][y] = 4;
		}
	}
}
