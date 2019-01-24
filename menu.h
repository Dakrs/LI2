#include "stdlib.h"
#include "stdio.h"
#include "cgi.h"
#include "estado.h"

/**
@file menu.h
Modulo responsável por representar os menus
*/

#define MAX_BUFFER		10240

/**
\brief Função que cria os link para mudar entre menus.
@param tipo Tipo de mudança
@param str Texto
@param x Coordenada x da representação
@param y Coordenada y da representação
*/

void startgame (int tipo, char * str, int x, int y);

/**
\brief Função que desenho o menu inicial.
*/

void menuDraw ();

/**
\brief Função que desenha o menu gameover.
@param score Score do jogador
*/
void gameOverDraw (int score);

/**
\brief Função responsável por criar o javascript para o teclado funcionar
*/

void KEYBOARD_MOVES ();

/**
\brief Função que gera os comando html necessários para começar a página
*/
void startHTML ();

/**
\brief Função que gera o texto para as ajudas do jogo
*/
void imprime_AJUDAS_GUIDES ();

/**
\brief Função que gera o javascript para a leitura que um elemento inserido pelo utilizador
*/

void javascript2 ();

/**
\brief Função responsável por imprimir o html que gera a label onde o utilizador escreve o nome de utilizador
*/

void imprime_LABEL_INPUT();

/**
\brief Função responsável por desenhar a parte relativa às keybinds
*/

void imprime_AJUDA_KEYBINDS ();

/**
\brief Função responsável por gera o menu para o utilizador introduzir o usernama
*/
void imprime_PAGINA_AJUDAS ();

/**
\brief Função responsável por criar o menu dos tesouros para apanhar habilidades
@param e Estado do jogo
*/

void draw_TREASURE_MENU (ESTADO e);
