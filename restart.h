#include <stdlib.h>

#include "estado.h"

/**
@file restart.h
Módulo para a geração do jogo
*/

/**
\brief Macro para o tamanho da Coordenada X do jogo.
*/
#define TAM				13

/**
\brief Macro para o tamanho da Coordenada Y do jogo.
*/

#define TAM2 			11

/**
\brief Macro para o número de inimigosmax.
*/

#define INIMIGOSMAX	12

/**
\brief Macro para o número de inimigo inicial.
*/

#define INIMIGOSATUAL	3

/**
\brief Função que verifica se uma posição é igual à posição da saida.
@param e Estado do jogo.
@param x Coordenada x da posição a verificar
@param y Coordenada y da posição a verificar
*/

int posicaofim(ESTADO e, int x,int y);

/**
\brief Função que verifica se duas posições são iguais.
@param p Posicao a comparar.
@param x Coordenada x da posição a verificar
@param y Coordenada y da posição a verificar
*/

int posicao_igual(POSICAO p, int x, int y);

/**
\brief Função que verifica se uma posição ta ocupada por uma poção.
@param e Estado do jogo
@param x Coordenada x da posição a verificar
@param y Coordenada y da posição a verificar
*/

int posicao_ocupadaPOT(ESTADO e, int x, int y);

/**
\brief Função que inicializa uma poção
@param e Estado do jogo
*/

ESTADO inicializar_pot(ESTADO e);

/**
\brief Função que inicializa um certo numero de poções
@param e Estado do jogo
@param x Número de poções
*/

ESTADO inicializar_pots(ESTADO e, int x);

/**
\brief Função que inicializa um obstaculo
@param e Estado do jogo
*/

ESTADO inicializar_obstaculo(ESTADO e);

/**
\brief Função que inicializa um inimigo
@param e Estado do jogo
@param SPEC Tipo de inimigo
*/

ESTADO inicializar_inimigo(ESTADO e , int SPEC);

/**
\brief Função que inicializa um certo numero de obstaculos
@param e Estado do jogo
@param num Número de obstaculos
*/

ESTADO inicializar_obstaculos(ESTADO e, int num);

/**
\brief Função que inicializa a saida
@param e Estado do jogo
*/

ESTADO inicializar_saida(ESTADO e);

/**
\brief Função que inicializa um certo numero de inimigos
@param e Estado do jogo
@param num Número de Inimigos
*/

ESTADO inicializar_inimigos(ESTADO e, int num);

/**
\brief Função que inicializa os stats do jogador
@param e Estado do jogo
*/

ESTADO iniciar_stats (ESTADO e);

/**
\brief Função que inicializa o tesouro
@param e Estado do jogo
*/

ESTADO inicializar_chest(ESTADO e);

/**
\brief Função que cria um estado
*/

ESTADO inicializar();

/**
\brief Função que gera um novo nivel
@param e Estado do jogo anterior
*/

ESTADO reinicializa (ESTADO e);

/**
\brief Função que verifica se existe um inimigo numa posicao.
@param e Estado do jogo
@param x Coordenada x
@param y Coordenada y
*/

int tem_inimigo (ESTADO e, int x, int y);

/**
\brief Função que verifica se existe um obstaculo numa posicao.
@param e Estado do jogo
@param x Coordenada x
@param y Coordenada y
*/

int tem_obstaculo (ESTADO e, int x, int y);

/**
\brief Função que verifica se existe jogador numa posicao.
@param e Estado do jogo
@param x Coordenada x
@param y Coordenada y
*/

int tem_jogador(ESTADO e, int x, int y);

/**
\brief Função que verifica se uma posicao está ocupada
@param e Estado do jogo
@param x Coordenada x
@param y Coordenada y
*/

int posicao_ocupada (ESTADO e, int x, int y);

/**
\brief Função que adiciona uma poção ao jogo
@param e Estado do jogo
*/

ESTADO addpot(ESTADO e);

/**
\brief Função elimina poções do jogo
@param e Estado do jogo
@param identificador Numero da poção
*/

ESTADO apagaPOTS (ESTADO e, int identificador);

/**
\brief Função modifica a duração das poções e as elimina
@param e Estado do jogo
*/

ESTADO modificaPOTS(ESTADO e);

/**
\brief Função responsável por apanhar um poção e produzir o seu efeito.
@param e Estado do jogo
@param x Coordenada x da celula a verificar
@param y Coordenada y da celula a verificar
*/

ESTADO apanhapot(ESTADO e, int x, int y);
