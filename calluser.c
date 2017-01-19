#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "calluser.h"

int callIntUser(char val){
	char quest[256];
	switch(val){
		case 'H'://horitontal
			strcpy(quest,"Donnez une taille horizontale : ");
		break;
		case 'V'://vertical
			strcpy(quest,"DOnnez une taille verticale : ");
		break;
		case 'b'://x block
			strcpy(quest,"Donnez une valeur d'apparition en x pour le block : ");
		break;
		case 'B'://y block
			strcpy(quest,"Donnez une valeur d'apparition en y pour le block : ");
		break;
		case 'p'://x poursuivant
			strcpy(quest,"Donnez une valeur d'apparition en x pour le poursuivant : ");
		break;
		case 'P'://y poursuivant
			strcpy(quest,"Donnez une valeur d'apparition en y pour le poursuivant : ");
		break;
		default:
			return 0;
	}
	int result;
	printf(quest);
	scanf("%d", &result);
	return result;
}

int callDefaultUser(){
	char res[3];
	printf("Voulez vous configurer le tableau et la position du block/poursuivant ? (yes/no)");
	scanf("%s", res);

	if(res[0] == 'y'){
		return 0;
	}
	else{
		return 1;
	}
}
