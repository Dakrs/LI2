#include "estado.h"
#include "stdlib.h"

/**
@file pathfinder.h
Modulo com o pathfinder
*/

/**
\brief Macro com o tamanho do x do mapa
*/

#define TAM				13

/**
\brief Macro com o tamanho do y do mapa
*/

#define TAM2 			11

/**
\brief Estrutura para a matriz do pathfinder
*/

typedef struct matriz
{
	int map[TAM2][TAM];
} * MATRIZ;

/**
\brief Função para validar um movimento nos limites do mapa
@param x Coordenada x
@param y Coordenada y
*/

int posicao_valida (int x, int y);

/**
\brief Função para validar se o movimento é possivel numa casa par
@param x Deslocamento x
@param y Deslocamento y
*/

int validaMovHexPar(int x,int y);

/**
\brief Função para validar se o movimento é possivel numa casa impar
@param x Deslocamento x
@param y Deslocamento y
*/

int validaMovHexImp(int x,int y);

/**
\brief Função para validar um movimento por ser feito
@param coordX Coordenada x
@param x Deslocamento x
@param y Deslocamento y
*/

int validaTudo(int coordX,int x, int y);

/**
\brief Função verifica se um arqueiro pode atacar
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int verificaATK (ESTADO e, int x, int y);

/**
\brief Função que preenche os valor da matriz do pathfinder dado um ponto
@param matriz Matriz pathfinder
@param x Coordenada x
@param y Coordenada y
*/

MATRIZ preencher (MATRIZ matriz, int x, int y);

/**
\brief Função que gera o pathfinder
@param e Estado do jogo
*/

MATRIZ pathfinding (ESTADO e);

/**
\brief Função responsável por verificar se um inimigo melee pode atacar
@param e Estado do Jogo
@param num_inimigo Número do Inimigo
*/

int checkMove (ESTADO e, int num_inimigo);

/**
\brief Função responsável pela 1º decisão no movimento do arqueiro
@param e Estado do Jogo
@param num_inimigo Número do Inimigo
@param mat Matriz do pathfinder
*/

ESTADO mobArcherXY (ESTADO e, int num_inimigo, MATRIZ * mat);

/**
\brief Função responsável pelo movimento dos monstros
@param e Estado do Jogo
@param num_inimigo Número do Inimigo
@param mat Matriz do pathfinder
*/

ESTADO mobMoveXY (ESTADO e, int num_inimigo, MATRIZ * mat);

/**
\brief Função verifica se um arqueiro ataca para a direita e para cima
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/
int coordUP_RIGHT (ESTADO e, int x, int y);

/**
\brief Função verifica se um arqueiro ataca para a direita e para baixo
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int coordDOWN_RIGHT (ESTADO e, int x, int y);

/**
\brief Função verifica se um arqueiro ataca para a esquerda e para cima
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int coordUP_LEFT (ESTADO e, int x, int y);

/**
\brief Função verifica se um arqueiro ataca para a esquerda e para baixo
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int coordDOWN_LEFT (ESTADO e, int x, int y);

/**
\brief Função verifica se um arqueiro ataca para cima na coluna
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int coordUP (ESTADO e, int x, int y);

/**
\brief Função verifica se um arqueiro ataca para baixo na coluna
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

int coordDOWN (ESTADO e, int x, int y);
