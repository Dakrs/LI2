#include "stdio.h"
#include "string.h"
#include "cgi.h"
#include "estado.h"

/**
@file score.h
Modulo responsavel pela scoreboard
*/


#define MAX_BUFFER		10240

/**
\brief Estrutura que tem um score e nome do utilizador
*/
typedef struct singlescore
{
	int valor;
	char nome[11];
}SSCORE;


/**
\brief Estrutura que armazena a scoreboard
*/

typedef struct score
{
	int num_score;
	SSCORE v[10];
} SCORE;

/**
\brief Função que passa o ficheiro local para o estado
@param fp ficheiro local
*/

SCORE str2scoreB (FILE * fp);

/**
\brief Função escreve num ficheiro local a scoreboard atual.
@param e Scoreboard do jogo
@param fp Ficheiro local.
*/

void score2strB (SCORE e, FILE * fp);


/**
\brief Função que insere um valor na scoreboard
@param valor Valor a introduzir
@param scoreboard Scoreboard atual
*/

void escreveSCORE (int valor , char * nome , SCORE scoreboard);

/**
\brief Função que escreve o nome do jogador no estado
@param e Estado do jogo
@param nome Nome do Utilizador
*/
ESTADO escreve_NOME_JOGADOR (ESTADO e, char * nome);

/**
\brief Função que le a scoreboard
*/
SCORE ler_score ();

/**
\brief Função que desenha a scoreboard
*/

void scoreDraw ();
