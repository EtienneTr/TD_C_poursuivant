#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "poursuivant.h"
#include "calluser.h"

void initPoursuivant(int autom){
	//manuel
	if(autom == 0){
		xPours = callIntUser('p');
		yPours = callIntUser('P');
	}else{
		srand(time(NULL));
		//on évite de pop sur la même case
		do{
			xPours = rand()%(TAILLE_VE-1) +1;
			yPours = rand()%(TAILLE_HO-1) +1;
		} while (xPours == xBlock && yPours == yBlock);
	}
	tab[xPours][yPours] = 3;
}

//calcul la posiiton du poursuivant pour rattraper le block.
void movePoursuivant(){
	//clear précédente position
	tab[xPours][yPours] = 0;
	//il peut se déplacer dans le carré autour de lui).
	
	//même x : on bouge le y
	if(xPours == xBlock){
		if(yPours < yBlock){
			yPours += 1;
		}else{
			yPours -= 1;
		}
	}
	//même y : on bouge x
	if(yPours == yBlock){
		if(xPours < xBlock){
			xPours += 1;
		}else{
			xPours -= 1;
		}
	}
	
	//diagonale
	//x- et y-
	if(xPours < xBlock && yPours < yBlock){
		xPours += 1;
		yPours += 1;
	}
	else if(xPours < xBlock && yPours > yBlock){
		xPours += 1;
		yPours -= 1;
	}
	else if(xPours > xBlock && yPours > yBlock){
		xPours -= 1;
		yPours -= 1;
	}
	else if(xPours > xBlock && yPours < yBlock){
		xPours -= 1;
		yPours += 1;
	}
	
	tab[xPours][yPours] = 3;
}
