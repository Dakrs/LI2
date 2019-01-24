#ifndef ___ESTADO_H___
#define ___ESTADO_H___

#include <stdio.h>
#include <stdlib.h>
/**
@file estado.h
Definição do estado e das funções que convertem estados em strings e vice-versa
*/

/** \brief O nº máximo de inimigos */
#define MAX_INIMIGOS		100

/** \brief O nº máximo de obstáculos */
#define MAX_OBSTACULOS		100

/** \brief O nº máximo de poções */
#define MAX_POTS 10

/**
\brief Estrutura que armazena uma posição
*/


typedef struct posicao {
	int x;
	int y;
} POSICAO;

/**
\brief Array com 12 posições
*/

typedef struct djump
{
	POSICAO v[12];
} DJMP;

/**
\brief Estrutura que armazena a informação relativa aos stats.
*/

typedef struct player
{
	int vida;
	int mana;
	int energy;
	int maxenergy;
	int maxvida;
	int maxmana;
} PLAYER;

/**
\brief Estrutura que armazena a informação sobre os feitiços e o equipamento
*/

typedef struct gear
{
	int shield;
	int voidf;
	int firef;
	int frostf;
} GEAR;

/**
\brief Estrutura que armazena a informação do jogador
*/

typedef struct ifull
{
	POSICAO posicao;
	POSICAO ant;
	PLAYER stats;
	GEAR armour;

} IFULL;

/**
\brief Estrutura que armazena a informação de um inimigo
*/

typedef struct ifullmobs
{
	POSICAO posicao;
	POSICAO ant;
	int ide;
	int guide;
	int spec;
} IFULLMOBS;

/**
\brief Estrutura que armazena a informação de um obstaculo
*/

typedef struct obstaculo
{
	POSICAO posicao;
	int state;
	int dur;
} OBS;

/**
\brief Estrutura que armazena a informação de uma poção
*/

typedef struct pots
{
	int type;
	int dur;
	POSICAO pos;
} POTS;

/**
\brief Estrutura que armazena a informação de um feitiço
*/

typedef struct spell
{
	POSICAO pos;
	int state;
} SPELL;


/**
\brief Estrutura que armazena a informação do tesouro.
*/

typedef struct chest
{
	POSICAO pos;
	int state;
	int gear;
} CHEST;
/**
\brief Estrutura que armazena o estado do jogo
*/
typedef struct estado {
	IFULL jog;
	int num_inimigos;
	int num_obstaculos;
	IFULLMOBS inimigo[MAX_INIMIGOS];
	OBS obstaculo[MAX_OBSTACULOS];
	POSICAO fim;
	int num_pots;
	POTS pocao[MAX_POTS];
	int menu;
	int score;
	int rscore;
	int turno;
	int singleguide;
	int multiguide;
	int doublejump;
	int freezepower;
	int flamepower;
	int attackAUX;
	int level;
	int voidspell;
	SPELL vortex;
	CHEST trea;
	POSICAO auxATK;
	int specatk;
	int treasurestate;
	int gameover;
	int ajudas;
	char nome_jogador[11];
	int byte;
} ESTADO;


/**
\brief Função que le um ficheiro local e returna o estado guardado.
@param fp Ficheiro local lido.
*/

ESTADO str2estadoB (FILE * fp);

/**
\brief Função escreve num ficheiro local o estado atual.
@param e Estado do jogo
@param fp Ficheiro local.
*/

void estado2strB (ESTADO e, FILE * fp);




#endif
