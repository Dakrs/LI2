#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "cgi.h"
#include "estado.h"
#include "pathfinder.h"
#include "restart.h"
#include "animation.h"
#include "score.h"
#include "menu.h"


/**
@file exemplo.c
Modulo Principal com a Main
*/

#define MAX_BUFFER		10240
#define TAM				13
#define TAM2 			11

void singleHexa (int x,int y,int t);

void singleHexa2 (int x,int y);

void singleHexa3 (int x,int y);

/**
\brief Função responsável por desenhar a saída
@param x Coordenada x da saida
@param y Coordenada y da saida
*/

void imprime_saida(int x,int y) {
	int posicaoX;

	posicaoX = x * 42;

	if (x % 2)
		IMAGEMSS(posicaoX+10,48*(y)+24+9 + y,36,36,"saidaf.png");
	else
		IMAGEMSS(posicaoX+10,48*(y)+9 + y,36,36,"saidaf.png");
}

/**
\brief Função responsável por retirar um inimigo do jogo
@param e Estado do Jogo
@param num_inimigo Número do inimigo
*/

ESTADO apagaInimigo (ESTADO e, int num_inimigo) {
	int i;

	for(i = num_inimigo; i < e.num_inimigos; i++) {
		e.inimigo[i] = e.inimigo[i+1];
	}
	e.num_inimigos--;

	return e;
}

/**
\brief Função responsável por tornar efetivo o efeito do spell vortex
@param e Estado do Jogo
@param x Coordenada x da celula atingida
@param y Coordenada y da celula atingida
*/

ESTADO apagaINIMIGO_VORTEX (ESTADO e, int x, int y) {
	int num_inimigo;

	if (tem_inimigo(e,x,y)) {
		for (num_inimigo = 0; !posicao_igual(e.inimigo[num_inimigo].posicao,x,y); num_inimigo++);

		e = apagaInimigo(e,num_inimigo);
	}

	return e;
}

/**
\brief Função faz o necessário relativo à Scoreboard para que esta funcione.
@param e Estado do jogo
*/

ESTADO togglescore (ESTADO e) {
	SCORE scoreboard;

	scoreboard = ler_score();
	escreveSCORE(e.rscore,e.nome_jogador,scoreboard);
	e = inicializar();

	return e;
}

/**
\brief Função que toma a decisão relativa à ação dos monstros
@param e Estado do jogo
*/

ESTADO mobMove (ESTADO e) {
	int num_inimigo, num_pot;
	POSICAO posicao_Inimigo;
	MATRIZ mat;

	for(num_inimigo=0; num_inimigo < e.num_inimigos; num_inimigo++) {
		mat = pathfinding(e);
		if(e.inimigo[num_inimigo].spec == 1){
			if (!checkMove(e,num_inimigo)) {
				e.jog.stats.vida -= 6 - e.jog.armour.shield;
				e.inimigo[num_inimigo].ant = e.inimigo[num_inimigo].posicao;
			}
			else {
				e = mobMoveXY(e,num_inimigo,&mat);
				if (posicao_ocupadaPOT(e,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y)) {
					for (num_pot = 0; !posicao_igual(e.pocao[num_pot].pos,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y) ;num_pot++);

					e = apagaPOTS(e,num_pot);
					num_pot = 0;
				}
			}
		}
		if(e.inimigo[num_inimigo].spec == 2){
			if (verificaATK(e,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y)) {
				e.jog.stats.vida -= 4 - e.jog.armour.shield;
				e.inimigo[num_inimigo].ant = e.inimigo[num_inimigo].posicao;
				e.inimigo[num_inimigo].ide = 1;
			}
			else {
				posicao_Inimigo = e.inimigo[num_inimigo].posicao;
				e = mobArcherXY(e,num_inimigo,&mat);
				if (posicao_igual(posicao_Inimigo,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y))
					e = mobMoveXY(e,num_inimigo,&mat);
				e.inimigo[num_inimigo].ide = 0;
				if (posicao_ocupadaPOT(e,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y)) {
					for (num_pot = 0; !posicao_igual(e.pocao[num_pot].pos,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y) ;num_pot++);
					e = apagaPOTS(e,num_pot);
					num_pot = 0;
				}
			}
		}
		free(mat);
	}

	return e;
}

/**
\brief Função conclui o ataque do jogador e faz as devidas alterações ao estado
@param e Estado do jogo
@param x Coordenada x do inimigo a ser atacado
@param y Coordenada y do inimigo a ser atacado
*/

ESTADO startBattle (ESTADO e, int x, int y){
	int num_inimigo;

	for (num_inimigo = 0; !posicao_igual(e.inimigo[num_inimigo].posicao,x,y) ; num_inimigo++);

	e.specatk = e.inimigo[num_inimigo].spec;
	e = apagaInimigo(e,num_inimigo);
	e.rscore += 1;
	e = mobMove(e);

	return e;
}

/**
\brief Função dado um movimento atriu um id para os links
@param v String que contem o id do link
@param dx Movimento no eixo x
@param dy Movimento no eixo y
*/

void cal_ID_MOVP(char * v, int dx, int dy) {
	if (dx == 0 && dy == -1)
		v[0] = 'u';
	else if (dx == 0 && dy == 1)
		v[0] = 'd';
	else if (dx==1 && dy== -1)
		v[0] = 'z';
	else if (dx == 1 && dy == 0)
		v[0] = 'k';
	else if (dx == -1 && dy == -1)
		v[0] = 'y';
	else if (dx == -1 && dy == 0)
		v[0] = 'x';
}

/**
\brief Função dado um movimento atriu um id para os links
@param v String que contem o id do link
@param dx Movimento no eixo x
@param dy Movimento no eixo y
*/

void cal_ID_MOVIMP(char * v, int dx, int dy) {
	if (dx == 0 && dy == -1)
		v[0] = 'u';
	else if (dx == 0 && dy == 1)
		v[0] = 'd';
	else if (dx==-1 && dy== 0)
		v[0] = 'y';
	else if (dx == -1 && dy == 1)
		v[0] = 'x';
	else if (dx == 1 && dy == 0)
		v[0] = 'z';
	else if (dx == 1 && dy == 1)
		v[0] = 'k';
}

/**
\brief Função que calcula o id dos link para usar o teclado.
@param v String com id do link
@param dx Movimento no eixo x
@param dy Movimento no eixo y
@param coordX Coordenada x
*/

void calID_MOV(char * v, int dx, int dy, int coordX) {
	if (coordX % 2)
		cal_ID_MOVIMP(v,dx,dy);
	else cal_ID_MOVP(v,dx,dy);
}

/**
\brief Função responsável por imprimir um link para um Movimento
@param e Estado do jogo
@param dx Movimento no eixo x
@param dy Movimento no eixo y
*/

void imprime_movimento(ESTADO e, int dx, int dy) {
	ESTADO novo;
	int x, y;
	char link[MAX_BUFFER];
	char v[2];

	calID_MOV(v,dx,dy,e.jog.posicao.x);
	v[1] = '\0';

	novo = e;
	x = e.jog.posicao.x + dx;
	y = e.jog.posicao.y + dy;

	if(!posicao_valida(x, y) || (tem_obstaculo(e,x,y) || tem_jogador(e,x,y)))
		return;

	if(tem_inimigo(novo, x, y))
		sprintf(link, "http://localhost/cgi-bin/perihelion?%s,%d,%d","atk",x,y);
	else
		sprintf(link, "http://localhost/cgi-bin/perihelion?%s,%d,%d","mov",x,y);

	ABRIR_LINK2(link,v);
	singleHexa(x,y,0);
	FECHAR_LINK;
}

/**
\brief Função que calcula as coordenadas afetadas pelo double jump quando o jogador está no posição impar.
*/

DJMP C_DJM_PAR () {
	DJMP t;
	t.v[0].x = 0;
	t.v[0].y = -2;
	t.v[1].x = -1;
	t.v[1].y = -2;
	t.v[2].x = -2;
	t.v[2].y = -1;
	t.v[3].x = -2;
	t.v[3].y = 0;
	t.v[4].x = -2;
	t.v[4].y = 1;
	t.v[5].x = -1;
	t.v[5].y = 1;
	t.v[6].x = 0;
	t.v[6].y = 2;
	t.v[7].x = 1;
	t.v[7].y = 1;
	t.v[8].x = 2;
	t.v[8].y = 1;
	t.v[9].x = 2;
	t.v[9].y = 0;
	t.v[10].x = 2;
	t.v[10].y = -1;
	t.v[11].x = 1;
	t.v[11].y = -2;

	return t;
}
/**
\brief Função que calcula as coordenadas afetadas pelo double jump quando o jogador está no posição par.
*/

DJMP C_DJM_IMPAR () {
	DJMP t;
	t.v[0].x = 0;
	t.v[0].y = -2;
	t.v[1].x = -1;
	t.v[1].y = -1;
	t.v[2].x = -2;
	t.v[2].y = -1;
	t.v[3].x = -2;
	t.v[3].y = 0;
	t.v[4].x = -2;
	t.v[4].y = 1;
	t.v[5].x = -1;
	t.v[5].y = 2;
	t.v[6].x = 0;
	t.v[6].y = 2;
	t.v[7].x = 1;
	t.v[7].y = 2;
	t.v[8].x = 2;
	t.v[8].y = 1;
	t.v[9].x = 2;
	t.v[9].y = 0;
	t.v[10].x = 2;
	t.v[10].y = -1;
	t.v[11].x = 1;
	t.v[11].y = -1;

	return t;

}

/**
\brief Função que imprime o link para congelar um casa com o spell de freeze
@param e Estado do jogo
@param dx Movimento no eixo x
@param dy Movimento no eixo y
*/

void imprime_MOV_FREEZE(ESTADO e, int dx, int dy){
	int x, y;
	char link[MAX_BUFFER];

	x = e.jog.posicao.x + dx;
	y = e.jog.posicao.y + dy;

	if (!tem_obstaculo(e,x,y))
		return;

	sprintf(link,"http://localhost/cgi-bin/perihelion?%s,%d,%d","FRZ",x,y);

	ABRIR_LINK(link);
	singleHexa3(x,y);
	FECHAR_LINK;
}

/**
\brief Função que imprime todos os link possiveis para congelar casas com o spell freeze
@param e Estado do jogo
*/

void imprime_MOVS_FREEZE(ESTADO e) {
	int dx, dy;

	for(dy = -1; dy <= 1; dy++)
		for(dx = -1 ; dx <= 1; dx++)
			if ((dx != 0 || dy != 0) && validaTudo(e.jog.posicao.x,dx,dy))
				imprime_MOV_FREEZE(e,dx,dy);
}

/**
\brief Função que imprime o link para atacar uma casa com o spell Vortex
@param e Estado do jogo
@param x Coordenada x da casa
@param y Coordenada y da casa
*/

void imprime_VOID_SPELL_SINGLE (int x, int y) {
	char link[MAX_BUFFER];

	sprintf(link, "http://localhost/cgi-bin/perihelion?TGV,%d,%d",x,y);

	ABRIR_LINK(link);
	singleHexa3(x,y);
	FECHAR_LINK;
}

/**
\brief Função que imprime os links de todas as casa que o spell Vortex pode atacar
@param e Estado do jogo
*/

void imprime_VOID_SPELL_ALL (ESTADO e) {
	int dx, dy, i;
	DJMP t;

	for(dy = -1; dy <= 1; dy++)
		for(dx = -1; dx <= 1; dx++)
			if ((dx != 0 || dy !=0) && posicao_valida(e.jog.posicao.x + dx, e.jog.posicao.y+dy) && validaTudo(e.jog.posicao.x,dx,dy))
				imprime_VOID_SPELL_SINGLE(e.jog.posicao.x+dx,e.jog.posicao.y+dy);

	if (e.jog.posicao.x % 2)
		t = C_DJM_IMPAR();
	else
		t = C_DJM_PAR();

	for(i=0; i < 12; i++) {
		t.v[i].x += e.jog.posicao.x;
		t.v[i].y += e.jog.posicao.y;
		if (posicao_valida(t.v[i].x,t.v[i].y))
			imprime_VOID_SPELL_SINGLE(t.v[i].x,t.v[i].y);
	}
}

/**
\brief Função que imprime o link para salta para um casa com o spell DoubleJump
@param e Estado do jogo
@param x Coordenada x da casa
@param y Coordenada y da casa
*/

void imprime_MOV_DJUMP (ESTADO e, int x, int y) {
	char link[MAX_BUFFER];

	if ((!posicao_valida(x,y)) || posicao_ocupada(e,x,y) || posicao_igual(e.fim,x,y))
		return;

	sprintf(link, "http://localhost/cgi-bin/perihelion?%s,%d,%d","mov",x,y);

	ABRIR_LINK(link);
	singleHexa3(x,y);
	FECHAR_LINK;
}

/**
\brief Função que imprime todos os links da casa afetadas pelo spell DoubleJump
@param e Estado do jogo
*/

void imprime_MOVS_DJUMP(ESTADO e) {
	int i;
	DJMP t;

	if (e.jog.posicao.x % 2)
		t = C_DJM_IMPAR();
	else
		t = C_DJM_PAR();

	for(i=0; i < 12; i++) {
		t.v[i].x += e.jog.posicao.x;
		t.v[i].y += e.jog.posicao.y;
		imprime_MOV_DJUMP(e,t.v[i].x,t.v[i].y);
	}
}

/**
\brief Função gera a animação da explosao do spell burn
@param e Estado do jogo
@param dx Movimento no eixo x
@param dy Movimento no eixo y
@param i Numero da explosão
*/

void invoca_explosao (ESTADO e, int dx, int dy, int i) {
	int x, y;

	x = e.jog.posicao.x + dx;
	y = e.jog.posicao.y + dy;

	if (!e.flamepower)
		return;

	if(!posicao_valida(x, y) || (tem_obstaculo(e,x,y) || tem_jogador(e,x,y)))
		return;

	if (x % 2)
		explosao(x*42+12,48*y + y + 33,i);
	else explosao(x*42+12,48*y + y+ 9, i);
}

/**
\brief Função que imprime o link de todos os movimento possiveis para o jogador
@param e Estado do jogo
*/

void imprime_movimentos(ESTADO e) {
	int dx, dy;
	int i = 0;

	for(dy =-1; dy <= 1; dy++)	{
		for(dx = -1; dx <= 1; dx++)	{
			if (dx != 0 || dy != 0)	{
				if (e.jog.posicao.x % 2)	{
					if (validaMovHexImp(dx,dy))	{
						invoca_explosao(e,dx,dy,i);
						imprime_movimento(e,dx,dy);
					}
				}
				else if (validaMovHexPar(dx,dy))	{
					invoca_explosao(e,dx,dy,i);
					imprime_movimento(e,dx,dy);
				}
				i++;
			}
		}
	}
}

/**
\brief Função que desenha o spell vortex
@param e Estado do jogo
*/

void imprime_VORTEX (ESTADO e) {
	int x, y, p1, p2;

	calSingleCOORD(e.jog.posicao.x,e.jog.posicao.y,&p1,&p2);
	calPIXEL(&x,&y,e.jog.posicao,e.vortex.pos);
	DRAW_VOID_SPELL(p1,p2,x,y);

	if (tem_inimigo(e,e.vortex.pos.x,e.vortex.pos.y)) {
		calSingleCOORD(e.vortex.pos.x,e.vortex.pos.x,&p1,&p2);
		DRAW_ENEMY_AUX2(p1,p2,e.specatk);
	}
}

/**
\brief Função que desenha tudo o que tenha a ver com o jogador (Equipamento, spells)
@param e Estado do jogo
*/

void imprime_jogador(ESTADO e) {
	int posicao, x, y;

	posicao = e.jog.ant.x * 42;
	calPIXEL(&x,&y,e.jog.ant,e.jog.posicao);

	if (e.attackAUX)
		DRAW_FULL_ATK(e);
	else {
		if (e.jog.ant.x % 2) {
			IMAGEMJJ(posicao+6,49*e.jog.ant.y + 24 + 8 , 45, 38, "zelda1f.png");
			ANIMATIONJ(0,0,x,y);
		}
		else {
			IMAGEMJJ(posicao+6,49*e.jog.ant.y + 8, 45, 38, "zelda1f.png");
			ANIMATIONJ(0,0,x,y);
		}
		imprime_armour(e.jog, e.jog.ant.y);
	}

	if (e.singleguide || e.multiguide)
		return;

	if (e.doublejump)
		imprime_MOVS_DJUMP(e);
	else if (e.freezepower)
		imprime_MOVS_FREEZE(e);
	else if (e.voidspell)
		imprime_VOID_SPELL_ALL (e);
	else if (e.vortex.state) {
		imprime_VORTEX(e);
		imprime_movimentos(e);
	}
	else imprime_movimentos(e);
}

/**
\brief Função que descongela as celulas do mapa.
@param e Estado do jogo
*/

ESTADO descongela (ESTADO e) {
	int num_obstaculo, x, y;

	for(num_obstaculo = 0; num_obstaculo < e.num_obstaculos; num_obstaculo++) {
		x = e.obstaculo[num_obstaculo].posicao.x;
		y = e.obstaculo[num_obstaculo].posicao.y;
		if (e.obstaculo[num_obstaculo].state && e.obstaculo[num_obstaculo].dur > 0)
			e.obstaculo[num_obstaculo].dur--;
		else if (e.obstaculo[num_obstaculo].state && (!tem_jogador(e,x,y)))
			e.obstaculo[num_obstaculo].state = 0;
	}

	return e;
}

/**
\brief Função que congela uma celula do mapa
@param e Estado do jogo
@param x Coordenada x da celula
@param y Coordenada y da celula
*/

ESTADO congela (ESTADO e, int x, int y) {
	int num_obstaculo;

	for (num_obstaculo = 0; num_obstaculo < e.num_obstaculos && (!posicao_igual(e.obstaculo[num_obstaculo].posicao,x,y)) ; num_obstaculo++);

	e.obstaculo[num_obstaculo].state = 1;
	e.obstaculo[num_obstaculo].dur = 10;

	return e;
}

/**
\brief Função que coloca a posicao anterior dos inimigo igual à atual
@param e Estado do jogo
*/

void mete_POS_IGUAIS (ESTADO * e) {
	int num_inimigo;

	for(num_inimigo=0; num_inimigo < e->num_inimigos; num_inimigo++) {
		e->inimigo[num_inimigo].ant = e->inimigo[num_inimigo].posicao;
		e->inimigo[num_inimigo].ide = 0;
	}
}

/**
\brief Função que altera as ajudas de todos os inimigos para o valor pretendido.
@param e Estado do Jogo
@param valor Valor pretendido nas ajudas
*/

ESTADO mete_AJUDAS_X(ESTADO e, int valor) {
	int num_inimigo;

	for(num_inimigo=0; num_inimigo < e.num_inimigos; num_inimigo++)
		e.inimigo[num_inimigo].guide = valor;

	return e;
}

/**
\brief Função que altera as ajudas de um inimigo para o inverso
@param e Estado do Jogo
@param x Coordenada x do inimigo
@param y Coordenada y do inimigo
*/

ESTADO inverte_AJUDA_SINGULAR(ESTADO e, int x, int y) {
	int num_inimigo;

	for (num_inimigo = 0; !posicao_igual(e.inimigo[num_inimigo].posicao,x,y) ; num_inimigo++);

	e.inimigo[num_inimigo].guide = (!e.inimigo[num_inimigo].guide);

	return e;
}

/**
\brief Função que elimina os inimigos afetados pela spell da explosao
@param e Estado do Jogo
@param x Coordenada x do spell
@param y Coordenada y do spell
*/

ESTADO elimina_EXPLOSAO(ESTADO e, int x, int y) {
	int num_inimigo;

	if (tem_inimigo(e,x,y)) {
		for (num_inimigo = 0; !posicao_igual(e.inimigo[num_inimigo].posicao,x,y) ; num_inimigo++);
		e = apagaInimigo(e,num_inimigo);
		e.rscore++;
	}

	return e;
}

/**
\brief Função que elimina todos os inimigos afetados por todas as celulas com explosões
@param e Estado do Jogo
*/

ESTADO elimina_EXPLOSAO_ALL (ESTADO e) {
	int dx, dy;

	for(dy = -1; dy <= 1; dy++)
		for(dx = -1; dx <= 1; dx++)
			if ((dx != 0 || dy != 0) && validaTudo(e.jog.posicao.x,dx,dy))
				e = elimina_EXPLOSAO(e,e.jog.posicao.x+dx,e.jog.posicao.y+dy);

	return e;
}

/**
\brief Função que retira um chest do jogo depois de apanhado.
@param e Estado do jogo
@param x Coordenada x do jogador
@param y Coordenada y do jogador
*/

ESTADO apanhaCHEST (ESTADO e, int x, int y) {

	if (posicao_igual(e.trea.pos,x,y)) {
		if (e.trea.state == 1) {
			e.trea.state = 0;
			e.treasurestate = 1;
		}
	}

	return e;
}

/**
\brief Função que executa as alterações necessárias ao estado quando o comando é o de movimento
@param e Estado do jogo
@param x Nova Coordenada x do jogador
@param y Nova Coordenada y do jogador
*/

ESTADO movimento (ESTADO e, int x , int y) {

	if (posicaofim(e,x,y))
		e = reinicializa(e);
	else {
		e.jog.ant = e.jog.posicao;
		e.jog.posicao.x = x;
		e.jog.posicao.y = y;
		e.flamepower = 0;
		e.attackAUX = 0;
		e.voidspell = 0;
		e.freezepower = 0;
		e.vortex.state = 0;

		e = apanhaCHEST(e, x, y);
		if (e.doublejump) {
			e.doublejump = 0;
			e.jog.stats.energy -= 1;
		}

		if (posicao_ocupadaPOT(e, x, y))	{
			e = apanhapot(e, x, y);
		}

		e = mobMove(e);
		e.turno++;
		e = modificaPOTS(e);

		if (e.turno % 3 == 0 && (e.num_inimigos != 0))	{
			e = addpot(e);
		}
		e = descongela(e);
	}
	return e;
}

/**
\brief Função que executa as alterações necessárias ao estado quando o comando é o de ataque
@param e Estado do jogo
@param x Coordenada x atacada
@param y Coordenada y atacada
*/

ESTADO ataque (ESTADO e, int x , int y) {

	e.jog.ant = e.jog.posicao;
	e = startBattle(e,x,y);
	e = descongela(e);
	e = modificaPOTS(e);
	e.auxATK.x = x;
	e.auxATK.y = y;
	e.attackAUX = 1;
	e.flamepower = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.vortex.state = 0;
	e.freezepower = 0;

	if (e.turno % 3 == 0 && (e.num_inimigos != 0)){
		e = addpot(e);
	}

	e.turno++;

	return e;
}
/**
\brief Função que ativa as ajudas multiplas
@param e Estado do jogo
*/

ESTADO ajudaMULTIPLAS (ESTADO e) {

	e.singleguide = 0;
	e.multiguide = (!e.multiguide);
	e.jog.ant = e.jog.posicao;
	e.flamepower = 0;
	mete_POS_IGUAIS(&e);
	e.attackAUX = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.vortex.state = 0;
	e.freezepower = 0;

	if (e.multiguide)
		e = mete_AJUDAS_X(e,1);
	else
		e = mete_AJUDAS_X(e,0);

	return e;
}

/**
\brief Função que ativa as ajudas singular
@param e Estado do jogo
*/

ESTADO ajudasSINGULARES (ESTADO e) {

	e.multiguide = 0;
	e.jog.ant = e.jog.posicao;
	e.flamepower = 0;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.freezepower = 0;
	e.vortex.state = 0;
	mete_POS_IGUAIS(&e);
	e = mete_AJUDAS_X(e,0);
	e.singleguide = (!e.singleguide);

	return e;
}

/**
\brief Função que ativa a ajuda de um inimigo singular
@param e Estado do jogo
@param x Coordenada x a ativar
@param y Coordenada y a ativar
*/

ESTADO ajudaSINGULAR (ESTADO e, int x , int y) {

	e.jog.ant = e.jog.posicao;
	mete_POS_IGUAIS(&e);
	e.flamepower = 0;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.vortex.state = 0;
	e.freezepower = 0;
	e = mete_AJUDAS_X(e,0);
	e = inverte_AJUDA_SINGULAR(e,x,y);

	return e;
}

/**
\brief Função que ativa o doublejump
@param e Estado do jogo
*/

ESTADO doublejump (ESTADO e) {

	e.jog.ant = e.jog.posicao;
	mete_POS_IGUAIS(&e);
	e.flamepower = 0;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.vortex.state = 0;
	e.freezepower = 0;
	e.doublejump = (!e.doublejump);

	return e;
}

/**
\brief Função que alterar o estado para congelar uma celula
@param e Estado do jogo
@param x Coordenada x da celula
@param y Coordenada y da celula
*/

ESTADO congelaDECODE (ESTADO e, int x , int y){

	e = congela(e,x,y);
	e.freezepower = (!e.freezepower);
	e.jog.ant = e.jog.posicao;
	mete_POS_IGUAIS(&e);
	e.jog.posicao.x = x;
	e.jog.posicao.y = y;
	e.flamepower = 0;
	e.attackAUX = 0;
	e.doublejump = 0;
	e.voidspell = 0;
	e.vortex.state = 0;
	e.jog.stats.mana -= 3;

	if (e.jog.stats.mana < 0)	{
		e.jog.stats.mana = 0;
	}
	e = mobMove(e);
	e = modificaPOTS(e);
	e.turno++;

	return e;

}

/**
\brief Função que ativa o freeze
@param e Estado do jogo
*/

ESTADO ativaFREEZE (ESTADO e) {

	e.jog.ant = e.jog.posicao;
	mete_POS_IGUAIS(&e);
	e.flamepower = 0;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.vortex.state = 0;
	e.doublejump = 0;
	e.freezepower = (!e.freezepower);

	return e;
}

/**
\brief Função que ativa executa o spell burn
@param e Estado do jogo
*/

ESTADO executaBURN (ESTADO e) {

	e = elimina_EXPLOSAO_ALL(e);
	e = descongela(e);
	e.flamepower = 0;
	mete_POS_IGUAIS(&e);
	e = mobMove(e);
	e = modificaPOTS(e);
	e.jog.ant = e.jog.posicao;
	e.jog.stats.mana -= 5;
	e.flamepower = 1;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.freezepower = 0;
	e.vortex.state = 0;

	if (e.jog.stats.mana < 0){
		e.jog.stats.mana = 0;
	}
	if (e.turno % 3 == 0 && (e.num_inimigos != 0)){
		e = addpot(e);
	}

	e.turno++;

	return e;
}

/**
\brief Função que ativa o spell vortex
@param e Estado do jogo
*/

ESTADO ativaVOID (ESTADO e) {

	e.voidspell = !e.voidspell;
	e.flamepower = 0;
	e.attackAUX = 0;
	e.doublejump = 0;
	e.freezepower = 0;
	e.vortex.state = 0;
	e.jog.ant = e.jog.posicao;
	mete_POS_IGUAIS(&e);

	return e;
}
/**
\brief Função que executa o spell vortex
@param e Estado do jogo
@param x Coordenada x do alvo
@param y Coordenada y do alvo
*/

ESTADO executaVORTEX (ESTADO e, int x, int y) {

	e.vortex.pos.x = x;
	e.vortex.pos.y = y;
	e.vortex.state = 1;
	e.jog.ant = e.jog.posicao;
	e.flamepower = 0;
	e.attackAUX = 0;
	e.voidspell = 0;
	e.doublejump = 0;
	e.freezepower = 0;
	e.jog.stats.mana -= 3;
	e = apagaINIMIGO_VORTEX(e,x,y);
	e = mobMove(e);
	e.turno++;
	e = modificaPOTS(e);

	if (e.turno % 3 == 0 && (e.num_inimigos != 0))	{
		e = addpot(e);
	}

	e = descongela(e);

	return e;
}


/**
\brief Função traduz o link recebido nas devidas alterações ao estado
@param e Estado do jogo
@param args Link
*/

ESTADO decodeEstado (ESTADO e, char *args) {
	int i, x, y;
	char v[4];

	for(i=0; i != 3; i++)
		v[i] = args[i];

	v[i] = '\0';

	if (strcmp(v,"mov") == 0) {
		sscanf(args,"mov,%d,%d",&x,&y);
		e = movimento(e,x,y);
	}
	else if (strcmp(v,"atk") == 0){
		sscanf(args,"atk,%d,%d",&x,&y);
		e = ataque(e,x,y);
	}
	else if (strcmp(v,"st1") == 0) {
		e.score = 1;
	}
	else if (strcmp(v,"st2") == 0) {
		e.score = 0;
		e.menu = 1;
	}
	else if (strcmp(v,"st3") == 0) {
		e.score = 0;
		e.menu = 0;
		e.ajudas = 1;
	}
	else if (strcmp(v,"gMT") == 0) {
		e = ajudaMULTIPLAS(e);
	}
	else if (strcmp(v,"gSG") == 0) {
		e = ajudasSINGULARES(e);
	}
	else if (strcmp(v,"CHG") == 0) {

		sscanf(args,"CHG,%d,%d",&x,&y);
		e = ajudaSINGULAR(e,x,y);

	}
	else if (strcmp(v,"DMJ") == 0) {
		e = doublejump(e);
	}
	else if (strcmp(v,"FRZ") == 0) {

		sscanf(args,"FRZ,%d,%d",&x,&y);
		e = congelaDECODE(e,x,y);

	}
	else if (strcmp(v,"TFR") == 0) {
		e = ativaFREEZE(e);
	}
	else if (strcmp(v,"BRN") == 0) {
		e = executaBURN(e);
	}
	else if (strcmp(v,"RST") == 0) {
		e = reinicializa(e);
	}
	else if (strcmp(v,"VOD") == 0) {
		e = ativaVOID(e);
	}
	else if (strcmp(v,"TGV") == 0) {

		sscanf(args,"TGV,%d,%d",&x,&y);
		e = executaVORTEX(e,x,y);
	}
	else if (strcmp(v,"vid") == 0) {

		e.jog.stats.maxvida += 1;
		e.jog.stats.vida += 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"vsf") == 0) {

		e.jog.armour.voidf = 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"fst") == 0) {

		e.jog.armour.frostf = 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"isf") == 0) {

		e.jog.armour.firef = 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"sss") == 0) {

		e.jog.armour.shield += 1;
		if (e.jog.armour.shield > 3){
			e.jog.armour.shield = 3;
		}
		e.treasurestate = 0;

	}
	else if (strcmp(v,"ddj") == 0) {

		e.jog.stats.maxenergy += 1;
		e.jog.stats.energy += 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"fff") == 0) {

		e.jog.stats.maxmana += 1;
		e.jog.stats.mana += 1;
		e.treasurestate = 0;

	}
	else if (strcmp(v,"aaa") == 0){
		e = togglescore(e);
	}

	else if (strcmp(v,"nam") == 0){
		e.menu = 0;
		e.score = 0;
		e.ajudas = 0;
		e = escreve_NOME_JOGADOR(e,args+3);
	}

	if (e.jog.stats.vida <= 0)
		e.gameover = 1;

	return e;
}

/**
\brief Função responsável por ler o ficheiro local onde o jogo esta guardado e altera-lo consuante o link recebido.
@param args Link
*/

ESTADO ler_estado(char *args) {
	ESTADO e;
	FILE * gamestatus;
	gamestatus = fopen("/var/www/html/ficheiros/gamestatus.txt", "r");

	if (!gamestatus)
		e = inicializar();
	else {
		e = str2estadoB(gamestatus);
	}

	e = decodeEstado(e,args);
	gamestatus = fopen("/var/www/html/ficheiros/gamestatus.txt", "w");
	estado2strB(e,gamestatus);
	fclose(gamestatus);
	return e;
}

/**
\brief Função responsável por desenhar os obstaculos.
@param e Estado do jogo
*/

void imprime_obstaculos(ESTADO e) {
	int num_obstaculo, posicao;

	for(num_obstaculo = 0; num_obstaculo < e.num_obstaculos; num_obstaculo++) {
		posicao = e.obstaculo[num_obstaculo].posicao.x * 42;
		if (e.obstaculo[num_obstaculo].state == 0) {
			if (e.obstaculo[num_obstaculo].posicao.x % 2) {
				 IMAGEMOO(posicao,49*(e.obstaculo[num_obstaculo].posicao.y)+24,num_obstaculo,56,49,"lava3ft.png");
				 singleHexa(e.obstaculo[num_obstaculo].posicao.x,e.obstaculo[num_obstaculo].posicao.y,1);
			}
			else {
				IMAGEMOO(posicao,49*(e.obstaculo[num_obstaculo].posicao.y),num_obstaculo,56,49,"lava3ft.png");
				singleHexa(e.obstaculo[num_obstaculo].posicao.x,e.obstaculo[num_obstaculo].posicao.y,1);
			}
		}
		else {
			if (e.obstaculo[num_obstaculo].dur != 10) {
				if (e.obstaculo[num_obstaculo].posicao.x % 2) {
					 IMAGEMOO(posicao,49*(e.obstaculo[num_obstaculo].posicao.y)+24,num_obstaculo,56,49,"freezef.png");
					 singleHexa(e.obstaculo[num_obstaculo].posicao.x,e.obstaculo[num_obstaculo].posicao.y,1);
				}
				else {
					IMAGEMOO(posicao,49*(e.obstaculo[num_obstaculo].posicao.y),num_obstaculo,56,49,"freezef.png");
					singleHexa(e.obstaculo[num_obstaculo].posicao.x,e.obstaculo[num_obstaculo].posicao.y,1);
				}
			}
			else {
				DRAW_SPELL_FREEZE(num_obstaculo,e.obstaculo[num_obstaculo].posicao.x,e.obstaculo[num_obstaculo].posicao.y);
			}
		}
	}
}

/**
\brief Função responsável por pintar as casas para a direita e para cima atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordUP_RIGHT_GUIDE (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x += 1;
    }
    else {
      x += 1;
      y -= 1;
    }

    if (i == 0) {
    	if (posicao_valida(x,y))
    		if (tem_inimigo(e,x,y))
    			return;
    }
    else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return;
    		singleHexa3(x,y);
    	}
    }
  }
}

/**
\brief Função responsável por pintar as casas para a direita e para baixo atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordDOWN_RIGHT_GUIDE (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x += 1;
      y += 1;
    }
    else {
      x += 1;
    }

    if (i == 0) {
    	if (posicao_valida(x,y))
    		if (tem_inimigo(e,x,y))
    			return;
    }
    else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return;
    		singleHexa3(x,y);
    	}
    }
  }
}

/**
\brief Função responsável por pintar as casas para a esquerda e para cima atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordUP_LEFT_GUIDE (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x -= 1;
    }
    else {
      x -= 1;
      y -= 1;
    }

    if (i == 0) {
    	if (posicao_valida(x,y))
    		if (tem_inimigo(e,x,y))
    			return;
    }
    else {
   		if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return;
    		singleHexa3(x,y);
    	}
    }
  }
}

/**
\brief Função responsável por pintar as casas para a esquerda e para baixo atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordDOWN_LEFT_GUIDE (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x -= 1;
      y += 1;
    }
    else {
      x -= 1;
    }
    if (i == 0) {
    	if (posicao_valida(x,y))
    		if (tem_inimigo(e,x,y))
    			return;
    }
    else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return;
    		singleHexa3(x,y);
   	 	}
   	}
  }
}

/**
\brief Função responsável por pintar as casas para cima atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordUP_GUIDE (ESTADO e, int x, int y) {
	int i;

	for(i=0; i < 4; i++) {
		y += 1;

		if (i== 0) {
			if (posicao_valida(x,y))
				if (tem_inimigo(e,x,y))
					return;
		}
		else {
			if (posicao_valida(x,y)) {
    			if (tem_inimigo(e,x,y))
    				return;
    			singleHexa3(x,y);
   	 		}
   	 	}
	}
}

/**
\brief Função responsável por pintar as casas para baixo atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void coordDOWN_GUIDE (ESTADO e, int x, int y) {
	int i;

	for(i=0; i < 4; i++) {
		y -= 1;

		if (i == 0) {
			if (posicao_valida(x,y))
				if (tem_inimigo(e,x,y))
					return;
		}
		else {
			if (posicao_valida(x,y)) {
   		 		if (tem_inimigo(e,x,y))
    				return;
    			singleHexa3(x,y);
    		}
    	}
	}
}

/**
\brief Função responáavel por pintar as casas para cima atacadas pelo arqueiro
@param e Estado do jogo
@param x Coordenada x do arqueiro
@param y Coordenada y do arqueiro
*/

void ajuda_ARQUEIRO(ESTADO e, int x, int y) {

	coordDOWN_LEFT_GUIDE(e,x,y);
	coordDOWN_RIGHT_GUIDE(e,x,y);
	coordUP_LEFT_GUIDE(e,x,y);
	coordUP_RIGHT_GUIDE(e,x,y);
	coordUP_GUIDE(e,x,y);
	coordDOWN_GUIDE(e,x,y);

}

/**
\brief Função responsável por pintar uma casa atacada por um melee
@param e Estado do jogo
@param x Coordenada x do inimigo
@param y Coordenada y do inimigo
*/

void ajuda_MELEE (ESTADO e, int x, int y) {

	if(posicao_valida(x,y) == 0 || tem_obstaculo(e,x,y) || tem_inimigo(e,x,y) || posicao_igual(e.fim,x,y))
		return;
	else
		singleHexa3(x,y);

}

/**
\brief Função responsável por pintar todas as casas atacadas por um melee
@param e Estado do jogo
@param x Coordenada x do inimigo
@param y Coordenada y do inimigo
*/

void ajuda_MELEE_ALL (ESTADO e,int x, int y) {
	int dx, dy;

	for(dy = -1; dy <= 1; dy++)
		for(dx = -1; dx <= 1; dx++)
			if ((dx != 0 || dy !=0) && validaTudo(x,dx,dy))
				ajuda_MELEE(e,x+dx,y+dy);

}

/**
\brief Função responsável por ativar as ajudas de um inimigo
@param e Estado do jogo
@param x Coordenada x do inimigo
@param y Coordenada y do inimigo
*/

void ativa_AJUDA (int x, int y) {
	char link [MAX_BUFFER];

	sprintf(link,"http://localhost/cgi-bin/perihelion?CHG,%d,%d",x,y);
	ABRIR_LINK(link);
	singleHexa(x,y,1);
	FECHAR_LINK;
}

/**
\brief Função responsável por desenhar todos os inimigos assim como as ajudas e as animações
@param e Estado do jogo
*/

void imprime_inimigos(ESTADO e) {
	int num_inimigo, posicao ,x ,y;

	for(num_inimigo = 0; num_inimigo < e.num_inimigos; num_inimigo++){

		calPIXEL(&x,&y,e.inimigo[num_inimigo].ant,e.inimigo[num_inimigo].posicao);
		posicao = e.inimigo[num_inimigo].ant.x * 42;

		if (e.inimigo[num_inimigo].ant.x % 2 && e.inimigo[num_inimigo].spec == 1) {
			 IMAGEMEE(posicao+6,48*(e.inimigo[num_inimigo].ant.y)+24+8+e.inimigo[num_inimigo].ant.y,num_inimigo,45,38,"mob1f.png");
			 ANIMATIONE(num_inimigo,0,0,x,y);
		}

		else if(e.inimigo[num_inimigo].ant.x % 2 == 0 && e.inimigo[num_inimigo].spec == 1){
			IMAGEMEE(posicao+6,48*(e.inimigo[num_inimigo].ant.y)+8+e.inimigo[num_inimigo].ant.y,num_inimigo,45,38,"mob1f.png");
			ANIMATIONE(num_inimigo,0,0,x,y);
		}

		else if(e.inimigo[num_inimigo].ant.x % 2 && e.inimigo[num_inimigo].spec == 2){
			IMAGEMEE(posicao+6,48*(e.inimigo[num_inimigo].ant.y)+24+e.inimigo[num_inimigo].ant.y + 4,num_inimigo,45,38,"archer1f.png");
			ANIMATIONE(num_inimigo,0,0,x,y);
			if (e.inimigo[num_inimigo].ide) {
				calSingleCOORD(e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y,&x,&y);
				PRINT_ARCHER_ATK(e.inimigo[num_inimigo].posicao,e.jog.posicao,x,y,num_inimigo);
			}
		}

		else if(e.inimigo[num_inimigo].ant.x % 2 == 0 && e.inimigo[num_inimigo].spec == 2){
			IMAGEMEE(posicao+6,48*(e.inimigo[num_inimigo].ant.y)+6+e.inimigo[num_inimigo].ant.y,num_inimigo,45,38,"archer1f.png");
			ANIMATIONE(num_inimigo,0,0,x,y);
			if (e.inimigo[num_inimigo].ide) {
				calSingleCOORD(e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y,&x,&y);
				PRINT_ARCHER_ATK(e.inimigo[num_inimigo].posicao,e.jog.posicao,x,y,num_inimigo);
			}
		}
	}

	for(num_inimigo=0; num_inimigo < e.num_inimigos; num_inimigo++) {
		if (e.inimigo[num_inimigo].guide) {
			if (e.inimigo[num_inimigo].spec == 1)
				ajuda_MELEE_ALL(e,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y);
			else ajuda_ARQUEIRO(e,e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y);
		}
		if (e.singleguide)
			ativa_AJUDA(e.inimigo[num_inimigo].posicao.x,e.inimigo[num_inimigo].posicao.y);
	}
}

/**
\brief Função que desenha uma poção dado um tipo e a sua posição.
@param tipo Tipo de poção
@param x Coordenada x
@param y Coordenada y
*/

void imprime_pot(int tipo, int x, int y) {

	if (tipo == 2)
		IMAGEMSS(x,y,45,38,"pots2,.png");
	else if (tipo)
		IMAGEMSS(x,y,45,38,"pots1,.png");
	else
		IMAGEMSS(x,y,45,38,"pots0,.png");
}

/**
\brief Função que desenha todas as poções do jogo.
@param e Estado do jogo
*/

void imprime_pots(ESTADO e) {
	int num_pot, posicao;

	for(num_pot=0; num_pot < e.num_pots; num_pot++){

		posicao = e.pocao[num_pot].pos.x * 42;

		if (e.pocao[num_pot].pos.x % 2)
			 imprime_pot(e.pocao[num_pot].type,posicao+7,48*(e.pocao[num_pot].pos.y)+24+e.pocao[num_pot].pos.y+5);

		else
			imprime_pot(e.pocao[num_pot].type,posicao+7,48*(e.pocao[num_pot].pos.y)+e.pocao[num_pot].pos.y+5);
	}
}

/**
\brief Função que desenha a vida do jogador.
@param vida Vida do jogador
@param maxvida Vida maxima do jogador
*/

void heartdraw (int vida, int maxvida) {
	int x, y, z;
	y = 50;
	z = 0;

	TEXT("Lives:",30,571,35,"#000000");

	for(x=0; x < maxvida; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(571 + 25*z,y,20,20,"heartf2.png");
		z++;
	}

	y = 50;
	z = 0;

	for(x=0; x < vida; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(571 + 25*z,y,20,20,"heartf.png");
		z++;
	}
}

/**
\brief Função que desenha a mana do jogador.
@param mana Mana do jogador
@param maxmana Mana maxima do jogador
*/

void manadraw (int mana, int maxmana) {
	int x, y, z;
	y = 50;
	z = 0;

	TEXT("Mana:",30,701,35,"#000000");

	for(x=0; x < maxmana; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(701 + 25*z,y,20,20,"manaslot2f.png");
		z++;
	}

	y = 50;
	z = 0;

	for(x=0; x < mana; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(701 + 25*z,y,20,20,"manaslotf.png");
		z++;
	}
}

/**
\brief Função que desenha a energia do jogador.
@param energy Energia do jogador
@param maxenergy Energia maxima do jogador
*/

void enegydraw (int energy, int maxenergy) {
	int x, y, z;
	y = 50;
	z = 0;

	TEXT("Energy:",30,831,35,"#000000");

	for(x=0; x < maxenergy; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(831 + 25*z,y,20,20,"botas2f.png");
		z++;
	}

	y = 50;
	z = 0;

	for(x=0; x < energy; x++) {
		if (x % 5 == 0 && x != 0) {
			z = 0;
			y += 25;
		}
		IMAGEMSS(831 + 25*z,y,20,20,"botasf.png");
		z++;
	}
}

/**
\brief Função que desenha os stats do jogador
@param e Estado do jogo
*/

void lifedraw (ESTADO e) {
	heartdraw(e.jog.stats.vida,e.jog.stats.maxvida);
	manadraw(e.jog.stats.mana,e.jog.stats.maxmana);
	enegydraw(e.jog.stats.energy,e.jog.stats.maxenergy);
}

/**
\brief Função que desenha o hexagono transparentes numa dada posição.
@param x Coordenada x da celula
@param y Coordenada y da celula
@param t Largura do stroke
*/

void singleHexa (int x,int y,int t) {
	int posicao;
	int posicao2;

	posicao = x * (1.5*28);
	posicao2 = (49*y);

	if (t)
		t=2;
	else
		t = 0;
	if (x % 2) {
		posicao2 += 24;
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0' stroke=%s stroke-width='%d' /> \n",posicao,posicao2+24, posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2+24,posicao+42,posicao2+48,posicao+14,posicao2+48,"#34282C","#000000",t);
	}
	else
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0'  stroke=%s stroke-width='%d' /> \n",posicao,posicao2+24,posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2 +24,posicao+42,posicao2 + 48, posicao + 14, posicao2 + 48,"#34282C","#000000",t);
}

/**
\brief Função responsavel por desenhar uma celula do background
@param x Coordenada x da celula
@param y Coordenada y da celula
*/

void singleHexa2 (int x,int y) {
	int posicao;
	int posicao2;

	posicao = x * (1.5*28);
	posicao2 = (49*y);

	if (x % 2) {
		posicao2 += 24;
		IMAGEMSS(posicao,posicao2,56,49,"hexa2f.png");
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0' stroke=%s stroke-width='2' /> \n",posicao,posicao2+24, posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2+24,posicao+42,posicao2+48,posicao+14,posicao2+48,"#34282C","#000000");
	}
	else {
		IMAGEMSS(posicao,posicao2,56,49,"hexa2f.png");
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0' stroke=%s stroke-width='2' /> \n",posicao,posicao2+24,posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2 +24,posicao+42,posicao2 + 48, posicao + 14, posicao2 + 48,"#34282C","#000000");
	}
}

/**
\brief Função que desenha os hexagonos para as ajudas aos jogadores numa dada posição.
@param x Coordenada x da celula
@param y Coordenada y da celula
*/

void singleHexa3 (int x,int y) {
	int posicao;
	int posicao2;

	posicao = x * (1.5*28);
	posicao2 = (49*y);

	if (x % 2) {
		posicao2 += 24;
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0.3' stroke=%s stroke-width='2' /> \n",posicao,posicao2+24, posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2+24,posicao+42,posicao2+48,posicao+14,posicao2+48,"#dbdb00","#000000");
	}
	else
		printf("<polygon points='%d,%d %d,%d %d,%d %d,%d %d,%d %d,%d' fill=%s fill-opacity='0.3'  stroke=%s stroke-width='2' /> \n",posicao,posicao2+24,posicao+14,posicao2,posicao+42,posicao2,posicao+56,posicao2 +24,posicao+42,posicao2 + 48, posicao + 14, posicao2 + 48,"#dbdb00","#000000");
}

/**
\brief Função que desenha o background do jogo.
*/

void hexaTotal() {
	int x, y;

	for(y=0; y < TAM2; y++)
		for(x=0; x < TAM; x++)
			singleHexa2(x, y);
}

/**
\brief Função responsável por ativar o poder da explosão.
@param e Estado do jogo
*/

void ativa_PODER_EXPLOSAO(ESTADO e){
	char link[MAX_BUFFER];

	if (e.jog.armour.firef)
		IMAGEMSS(761,300,80,80,"spell1f.png");

	if (e.jog.stats.mana < 5 || e.jog.armour.firef != 1)
		return;

	sprintf(link,"http://localhost/cgi-bin/perihelion?BRN");

	ABRIR_LINK(link);
	IMAGEMSS(761,300,80,80,"spell1f.png");
	FECHAR_LINK;

}

/**
\brief Função responsável por ativar o poder para congelar casas
@param e Estado do jogo
*/

void ativa_PODER_CONGELA(ESTADO e) {
	char link[MAX_BUFFER];

	if (e.jog.armour.frostf)
		IMAGEMSS(666,300,80,80,"spell3f.png");

	if (e.jog.stats.mana < 3 || e.jog.armour.frostf != 1)
		return;

	sprintf(link,"http://localhost/cgi-bin/perihelion?TFR");

	ABRIR_LINK(link);
	IMAGEMSS(666,300,80,80,"spell3f.png");
	FECHAR_LINK;
}

/**
\brief Função responsável por ativar o poder Vortex
@param e Estado do jogo
*/

void ativa_PODER_VORTEX(ESTADO e) {
	char link[MAX_BUFFER];

	if (e.jog.armour.voidf)
		IMAGEMSS(571,300,80,80,"spell2f.png");

	if (e.jog.stats.mana < 3 || e.jog.armour.voidf != 1)
		return;

	sprintf(link,"http://localhost/cgi-bin/perihelion?VOD");
	ABRIR_LINK(link);
	IMAGEMSS(571,300,80,80,"spell2f.png");
	FECHAR_LINK;
}

/**
\brief Função responsável por ativar o poder double jump.
@param e Estado do Jogo
*/

void ativa_PODER_DOUBLEJUMP(ESTADO e) {
	char link [MAX_BUFFER];

	if (e.jog.stats.energy == 0)
		IMAGEMSS(856,300,80,80,"doublejumpf.png");
	else {
		sprintf(link,"http://localhost/cgi-bin/perihelion?DMJ");

		ABRIR_LINK(link);
		IMAGEMSS(856,300,80,80,"doublejumpf.png");
		FECHAR_LINK;
	}
}

/**
\brief Função responsavel por ativar as ajudas.
@param tipo Tipo da ajuda
@param texto String de Texto
@param x Coordenada x da representação
@param y Coordenada y da representação
*/

void ativar_AJUDAS_FIRST(int tipo, char * texto, int x, int y) {
	char link [MAX_BUFFER];

	if (tipo)
		sprintf(link,"http://localhost/cgi-bin/perihelion?gSG");
	else
		sprintf(link,"http://localhost/cgi-bin/perihelion?gMT");

	ABRIR_LINK(link);
	TEXT(texto,30,x,y,"#D35400");
	FECHAR_LINK;
}

/**
\brief Função responsavel por desenhar o chest.
@param e Estado do jogo
*/

void PRINT_CHEST(ESTADO e) {
	int x, y;

	if (!e.trea.state)
		return;

	calSingleCOORD (e.trea.pos.x, e.trea.pos.y, &x, &y);
	IMAGEMSS(x+11,y+8,35,35,"chest1f.png");
}

/**
\brief Função cria os links para ativas os 2 tipos de ajudas
*/

void Draw_Guide () {

	TEXT("Enemy guides",35,571,400,"#D35400");
	ativar_AJUDAS_FIRST(1,"Single",571,425);
	ativar_AJUDAS_FIRST(0,"Multi",571,455);

}

/**
\brief Função principal para desenhar o jogo em si.
@param e Estado do Jogo
*/

void mapaDraw (ESTADO e)  {
	char string [40];

	ABRIR_SVG(1000,625);
	printf("<image x=0 y=0 width=1000 height=625 xlink:href='http://localhost/images/gameback.jpg' />\n");

	hexaTotal();
	imprime_saida(e.fim.x,e.fim.y);
	imprime_pots(e);
	imprime_obstaculos(e);
	lifedraw(e);
	PRINT_CHEST(e);
	if (e.multiguide || e.singleguide) {
		imprime_jogador(e);
		imprime_inimigos(e);
	}
	else {
		imprime_inimigos(e);
		imprime_jogador(e);
	}
	TEXT("Spell Learnt:",30,571,300,"#000000");
	ativa_PODER_DOUBLEJUMP(e);
	ativa_PODER_CONGELA(e);
	ativa_PODER_EXPLOSAO(e);
	ativa_PODER_VORTEX(e);
	Draw_Guide();
	sprintf(string,"%s Score: %d",e.nome_jogador,e.rscore);
	TEXT(string,30,571,260,"#000000");
	FECHAR_SVG;
}

/**
\brief Função main
*/

int main () {
	ESTADO e;

	srandom(time(NULL));

	e = (ler_estado(getenv("QUERY_STRING")));
	startHTML();

	if (e.score)
		scoreDraw();
	else if (e.gameover)
		gameOverDraw(e.rscore);
	else if (e.menu)
		menuDraw();
	else if (e.treasurestate)
		draw_TREASURE_MENU(e);
	else if (e.ajudas)
		imprime_PAGINA_AJUDAS();
	else
		mapaDraw(e);

	return 0;
}
