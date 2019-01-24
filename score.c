#include "score.h"


SCORE str2scoreB (FILE * fp) {
	SCORE e;
	int i;

	i = fread(&e, sizeof(SCORE),1, fp);
	e.num_score += i - i;
	
	return e;
}

void score2strB (SCORE e, FILE * fp) {
	fwrite(&e, sizeof(SCORE),1,fp);
}


void escreveSCORE (int valor , char * nome , SCORE scoreboard) {
	int i, k, z;
	SSCORE t, aux;
	FILE * score;

	for (i = 0; i < scoreboard.num_score && scoreboard.v[i].valor > valor; i++);

	scoreboard.num_score += 1;
	t = scoreboard.v[i];
	scoreboard.v[i].valor = valor;

	for(z=0; nome[z] != '\0' && z < 10; z++)
		scoreboard.v[i].nome[z] = nome[z];

	scoreboard.v[i].nome[z] = '\0';

	if (scoreboard.num_score > 10)
		scoreboard.num_score = 10;

	for(k = i; k < scoreboard.num_score - 1; k++) {
		aux = scoreboard.v[k+1];
		scoreboard.v[k+1] = t;
		t = aux;
	}

	score = fopen("/var/www/html/ficheiros/score.txt", "w");
	score2strB(scoreboard,score);
	fclose(score);
}

ESTADO escreve_NOME_JOGADOR (ESTADO e, char * nome) {
	int i;

	for(i=0 ; nome[i] != '\0' && i < 10; i++)
		e.nome_jogador[i] = nome[i];

	e.nome_jogador[i] = '\0';

	return e;
}

SCORE ler_score () {
	SCORE scoreboard;
	FILE * score;

	if ((score = fopen("/var/www/html/ficheiros/score.txt", "r"))) {
	 	score = fopen("/var/www/html/ficheiros/score.txt", "r");
		scoreboard = str2scoreB(score);
	}
	else {
		scoreboard.num_score = 0;
		memset(&scoreboard , 0 , sizeof(SCORE));

	 	score = fopen("/var/www/html/ficheiros/score.txt", "w");
	 	score2strB(scoreboard,score);
		}

	fclose(score);
	return scoreboard;
}

void scoreDraw () {
	SCORE scoreboard;
	int i;
	char texto[MAX_BUFFER];

	scoreboard = ler_score();
	ABRIR_SVG(1000,600);
	IMAGEMSS(-100,0,1000,600,"scoreboard.png");
	TEXT("Scoreboard",130,150,100,"#ffffff");

	for(i=0; i< scoreboard.num_score; i++) {
		sprintf(texto,"%d",scoreboard.v[i].valor);
		TEXT(texto,40,460,150 + 45*i,"#ffffff");
		TEXT(scoreboard.v[i].nome,40,330,150 + 45*i,"#ffffff");
	}

	sprintf(texto,"http://localhost/cgi-bin/perihelion?st2");

	ABRIR_LINK(texto);
	IMAGEMSS(725,5,50,50,"stopf.png");
	FECHAR_LINK;

	FECHAR_SVG;
}
