#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>

#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include "poursuivant.h"
#include "obstacles.h"
#include "calluser.h"

int TAILLE_HO = 50;
int TAILLE_VE = 30;

int DefaultConf = 1;

int **tab;
int xBlock = 0;
int yBlock = 0;
int xPours = 0;
int yPours = 0;
int go = 1;

char lastAction = 's';

int main(){
	//configuration auto (1) manuelle (0);
	DefaultConf = callDefaultUser();
	
	//manuel = on demande taille du tableau
	if(DefaultConf == 0){
		TAILLE_HO = callIntUser('H');
		TAILLE_VE = callIntUser('V');
	}

	//malloc tableau
	tab = malloc(TAILLE_VE * sizeof(*tab));

	for(int i=0 ; i < TAILLE_VE ; i++){
	     tab[i] = malloc(TAILLE_HO * sizeof(**tab));
	}

	initInterface();
	initObstacle();
	callUser();
}

//Init interface : création tableau avec mur, position du block
void initInterface(){

	printf("Interface %d * %d\n", TAILLE_HO, TAILLE_VE);

	//création mur
	for(int i=0; i<TAILLE_VE; i++){
		for(int j=0; j<TAILLE_HO; j++){
			if((j == 0 || (j == TAILLE_HO - 1)) || (i == 0 || (i == TAILLE_VE -1))){
				tab[i][j] = 1;
			}else{
				tab[i][j] = 0;
			}
			
		}
	}

	//manuel : position block
	if(DefaultConf == 0){
		xBlock = callIntUser('b');	
		yBlock = callIntUser('B');
	}else{

		//placement block aléatoire
		srand(time(NULL));
		xBlock = rand()%(TAILLE_VE-1) +1;
		yBlock = rand()%(TAILLE_HO-1) +1;
	}
	tab[xBlock][yBlock] = 2;

	//direction du block aléatoire
	int r = rand()%(5-1) +1;
	switch(r){
		case 1:
			lastAction = 'z';
		break;
		case 2:
			lastAction = 'd';
		break;
		case 3:
			lastAction = 's';
		break;
		default:
			lastAction = 'q';
	}

	//init Poursuivant
	initPoursuivant(DefaultConf);
	printTab();
}

//config terminal + lecture touches
void callUser() {
  int lu;
  struct termios tios;
  char c = 'A';

  // passage en mode -icanon -echo
  if (tcgetattr(fileno(stdin), &tios) < 0) exit(-1);
  tios.c_lflag &= ~(ECHO | ICANON);
  if (tcsetattr(fileno(stdin), TCSAFLUSH, &tios) < 0) exit(-1);

  if (fcntl(fileno(stdin), F_SETFL, O_NONBLOCK) < 0) {
    fprintf(stderr, "erreur fcntl stdin : %d", errno);
    exit(errno);
  }
  
  int inc = 0;
  while (go == 1) {
    lu = read(fileno(stdin), &c, 1);
    if (lu > 0) {
      //printf("\t\t%c\n", c);
      actionInterface(c);
      fflush(stdout);
    }else{
      actionInterface(lastAction);
    }
    //poursuivant avance
    checkPosition(inc);
    if(go == 0) break;
    movePoursuivant();
    //check and print
    checkPosition(inc);
    inc += 1;
    usleep(500000);
  }

  system("stty sane echo");
}

//calcul la position suivante du block en fonction de l'action en entrée (z, q, s, d)
void actionInterface(char value){
	int oldX;
	int oldY;
	switch(value){
		case 'z'://en haut
			oldX = xBlock - 1;
			if(oldX > 0){
				xBlock = oldX;
				tab[xBlock + 1][yBlock] = 0;
				tab[xBlock][yBlock] = 2;
			}
			lastAction = 'z';
		break;
		case 's'://en bas
			oldX = xBlock + 1;
			if(oldX < TAILLE_VE - 1){
				xBlock = oldX;
				tab[xBlock - 1][yBlock] = 0;
				tab[xBlock][yBlock] = 2;
			}
			lastAction = 's';
		break;
		case 'q'://gauche
			oldY = yBlock - 1;
			if(oldY >0){
				yBlock = oldY;
				tab[xBlock][yBlock + 1] = 0;
				tab[xBlock][yBlock] = 2;
			}
			lastAction = 'q';
		break;
		case 'd'://droite
			oldY = yBlock + 1;
			if(oldY < TAILLE_HO - 1){
				yBlock = oldY;
				tab[xBlock][yBlock - 1] = 0;
				tab[xBlock][yBlock] = 2;
			}
			lastAction = 'd';
		break;
		default:
		break;
	}
}

//vérifie si le poursuivant a rattrapé
void checkPosition(int tour){
	if(xBlock == xPours && yBlock == yPours){
		printf("\n\nPerdu ! Le poursuivant vous a rattrapé. \n");
		float time = tour*0.5;
		printf("Vous avez tenu %d tours soit %f secondes\n", tour,time);
		go = 0;		
	}
	else { 
		printTab();
	}
}

void printTab(){
	for(int i=0; i<TAILLE_VE; i++){
		for(int j=0; j<TAILLE_HO; j++){
			int val = tab[i][j];
			switch(val){
				case 1://mur
					printf("-");
				break;
				case 2://block
					printf("A");
				break;
				case 3://poursuivant
					printf("P");
				break;
				case 4://obstacles
					printf("O");
				break;
				default://vide
					printf(" ");
			}
		
		}
		printf("\n");
	}
}

