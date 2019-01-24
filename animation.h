#include <stdio.h>
#include <math.h>
#include "estado.h"
#include "cgi.h"
#include "restart.h"

/**
@file animation.h
Modulo responsável por criar todas as animações necessárias
*/


/**
\brief Valor do pi
*/
#define PI 3.14159265

/**
\brief Função que desenha a animação para um explosão.
@param x coordenadas onde é representada a imagem nos x
@param y coordenadas onde é representada a imagem nos y
@param identificador Identicador da imagem
*/

void explosao (int x, int y, int identificador);

/**
\brief Função que calcula a diferença de pixeis entre duas posições.
@param x Posição para guardar a diferença do x
@param y Posição para guardar a diferença do y
@param a Ponto inicial
@param b Ponto final
*/

void calPIXEL (int * x, int * y, POSICAO a,POSICAO b);

/**
\brief Função que ve o quadrante em que o movimento se encontra.
@param x Valor do x
@param y Valor do y
*/

int quadrante (int x,int y);

/**
\brief Função que dado as devidas coordenadas calcula a posição onde esta vai ser representada.
@param x Coordenadas x
@param y Coordenadas y
@param x1 Posição onde a coordenadas representada x é guardada
@param y1 Posição onde a coordenadas representada y é guardada
*/

void calSingleCOORD (int x, int y, int * x1, int * y1);

/**
\brief Função que calcula o angulo da rotação da imagem
@param x Coordenada x
@param y Coordenada y
*/

int calAGL (int x, int y);

/**
\brief Função que desenha a armadura do jogador assim como as suas animações
@param jog Informação sobre o jogador
@param ajuste Ajuste da imagem
*/

void imprime_armour(IFULL jog, int ajuste);

/**
\brief Função que desenha a imagem de ataque do jogador
@param angulo Angulo de rotação da imagem
@param x1 Coordenadas do x
@param y1 Coordenadas do y
*/

void PRINT_IMG_ATK (int angulo, int x1, int y1);

/**
\brief Função que desenha uma flecha do arqueiro
@param angulo Angulo de rotação da imagem
@param x Coordenadas do x
@param y Coordenadas do y
@param identificador Identificador da flecha
*/

void PRINT_ARROW(int angulo, int x, int y, int identificador);

/**
\brief Função que desenha a animação de ataque dos arqueiros
@param a Posição inicial
@param b Posição final
@param x Coordenadas do x
@param y Coordenadas do y
@param ajuste Ajuste da imagem
*/

void PRINT_ARCHER_ATK(POSICAO a,POSICAO b, int x , int y, int ajuste);

/**
\brief Função que desenha a animação de ataque do jogador
@param a Posição inicial
@param b Posição final
@param x Coordenadas do x
@param y Coordenadas do y
*/

void PRINT_IMG_ATK_ALL(POSICAO a,POSICAO b, int x, int y);

/**
\brief Função que desenha um inimigo auxilar para o ataque com um timer de 0.25
@param x Coordenadas do x
@param y Coordenadas do y
@param tipo Tipo do Inimigo
*/

void DRAW_ENEMY_AUX2 (int x,int y,int tipo);

/**
\brief Função que desenha um inimigo auxilar para o ataque com um timer de 0.4
@param x Coordenadas do x
@param y Coordenadas do y
@param tipo Tipo do Inimigo
*/

void DRAW_ENEMY_AUX (int x,int y, int tipo);

/**
\brief Função que desenha o jogador depois do ataque
@param x Coordenadas do x
@param y Coordenadas do y
*/

void DRAW_LAST_PLAYER (int x,int y);

/**
\brief Função que desenha o equipamento depois de atacar
@param x Coordenadas do x
@param y Coordenadas do y
*/

void DRAW_GEAR_ANI(int x,int y);

/**
\brief Função que desenha a animacao do spell vortex
@param x Coordenadas do x
@param y Coordenadas do y
@param z1 Deslocamento nos x
@param z2 Deslocamento nos y
*/

void DRAW_VOID_SPELL(int x,int y, int z1, int z2);

/**
\brief Função que desenha a animacao de congelar uma casa
@param identificador Identificador da celula
@param x Coordenadas do x
@param y Coordenadas do y
*/

void DRAW_SPELL_FREEZE(int identificador,int x,int y);

/**
\brief Função que desenha toda a animação de ataque do jogador
@param e Estado do jogo
*/

void DRAW_FULL_ATK (ESTADO e);
