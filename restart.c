#include "restart.h"
#include <string.h>


int posicaofim(ESTADO e, int x,int y)  {

	return ((e.fim.x == x) && (e.fim.y == y));

}

int posicao_igual(POSICAO p, int x, int y){

	return p.x == x && p.y == y;

}

int posicao_ocupadaPOT(ESTADO e, int x, int y) {
	int i;

	for(i=0; i < e.num_pots; i++)
		if (posicao_igual(e.pocao[i].pos,x,y))
			return 1;

	return 0;
}

ESTADO inicializar_pot(ESTADO e) {
	int x , y , z;

	z = random () % 3;

	do {
		x = random() % TAM;
		y = random() % TAM2;
	} while (posicao_ocupada (e, x, y) || posicao_ocupadaPOT(e, x, y) || posicao_igual(e.fim, x, y));

	e.pocao[e.num_pots].type = z;
	e.pocao[e.num_pots].dur = 10;
	e.pocao[e.num_pots].pos.x = x;
	e.pocao[e.num_pots].pos.y = y;
	e.num_pots++;

	return e;
}

ESTADO inicializar_pots(ESTADO e, int x) {
	int i;

	for(i = 0; i < x; i++)
		e = inicializar_pot(e);

	return e;
}

ESTADO inicializar_obstaculo(ESTADO e){
	int x , y;

	do {
		x = random() % TAM;
		y = random() % TAM2;
	} while (posicao_ocupada (e, x, y));

	e.obstaculo[e.num_obstaculos].posicao.x = x;
	e.obstaculo[e.num_obstaculos].posicao.y = y;
	e.num_obstaculos++;

	return e;
}

ESTADO inicializar_inimigo(ESTADO e, int SPEC){
	int x, y;

	do {
		x = random() % TAM;
		y = random() % TAM2;
	} while (posicao_ocupada (e, x, y));

	e.inimigo[e.num_inimigos].spec = SPEC + 1;
	e.inimigo[e.num_inimigos].posicao.x = x;
	e.inimigo[e.num_inimigos].posicao.y = y;
	e.inimigo[e.num_inimigos].ant.x = x;
	e.inimigo[e.num_inimigos].ant.y = y;
	e.num_inimigos++;

	return e;
}

ESTADO inicializar_obstaculos(ESTADO e, int num){
	int i;

	for(i=0; i < num; i++)
		e = inicializar_obstaculo(e);

	return e;
}

ESTADO inicializar_saida(ESTADO e) {

	e.fim.x = random() % TAM;
	e.fim.y = random() % TAM2;

	while(posicao_ocupada(e,e.fim.x,e.fim.y)) {
	     e.fim.x = random() % TAM;
	     e.fim.y = random() % TAM2;
	}

	return e;
}


ESTADO inicializar_inimigos(ESTADO e, int num){
	int i;

	for(i=0; i < num && i < INIMIGOSMAX; i++){
		if(i == 0 || i == 1)
			e = inicializar_inimigo(e , 0);
		else if (i == 2)
			e = inicializar_inimigo(e , 1);
		else
			e = inicializar_inimigo(e , random() % 2);
	}

	return e;
}

ESTADO iniciar_stats (ESTADO e) {

	e.jog.armour.shield = 1;
	e.jog.stats.vida=9;
	e.jog.stats.mana=9;
	e.jog.stats.maxvida = 9;
	e.jog.stats.maxmana = 9;
	e.jog.stats.energy = 2;
	e.jog.stats.maxenergy = 2;

	return e;
}

ESTADO inicializar_chest(ESTADO e) {
	int x, y;

	e.trea.gear = 0;
	e.trea.state = 1;

	do {
		x = random () % TAM;
		y = random () % TAM2;
	} while (posicao_ocupada(e,x,y) || posicao_igual(e.fim,x,y) || posicao_ocupadaPOT(e,x,y));

	e.trea.pos.x = x;
	e.trea.pos.y = y;

	return e;
}

ESTADO inicializar() {
	ESTADO e;
	int y;

	memset(&e , 0 , sizeof(ESTADO));
	y = 0;

	e.jog.posicao.x = random() % TAM;
	e.jog.posicao.y = random() % TAM2;
	e.jog.ant.x = e.jog.posicao.x;
	e.jog.ant.y = e.jog.posicao.y;

	while(y < 20)
		y = random() % 30;

	e = iniciar_stats(e);
	e = inicializar_inimigos (e, INIMIGOSATUAL);
	e = inicializar_obstaculos (e, y);
	e = inicializar_saida(e);
	e = inicializar_pots(e,5);
	e = inicializar_chest(e);
	e.menu = 1;
	e.score = 0;
	e.rscore = 0;

	return e;
}

ESTADO reinicializa (ESTADO e) {
	int y, score, turn;
	ESTADO novo;
	IFULL jog;

	jog = e.jog;
	turn = e.level;
	y = 0;
	score = e.rscore;

	memset(&novo , 0 , sizeof(ESTADO));

	novo.jog = jog;
	novo.jog.posicao.x = random() % TAM;
	novo.jog.posicao.y = random() % TAM2;
	novo.jog.ant.x = novo.jog.posicao.x;
	novo.jog.ant.y = novo.jog.posicao.y;
	novo.level = turn + 1;

	while(y < 20) {
		y = random() % 30;
	}

	novo = inicializar_inimigos (novo, novo.level + INIMIGOSATUAL);
	novo = inicializar_obstaculos (novo,y);
	novo = inicializar_saida(novo);
	novo = inicializar_pots(novo,5);
	novo.menu = 0;
	novo.score = 0;
	novo.rscore = score;
	novo = inicializar_chest(novo);

	for(y=0; e.nome_jogador[y] != '\0' && y < 10; y++)
		novo.nome_jogador[y]= e.nome_jogador[y];

	novo.nome_jogador[y] = '\0';

	return novo;

}


int tem_inimigo (ESTADO e, int x, int y){
	int i;

	for(i = 0; i < e.num_inimigos; i++)
		if(posicao_igual (e.inimigo[i].posicao, x, y))
			return 1;

	return 0;
}


int tem_obstaculo (ESTADO e, int x, int y){
	int i;

	for(i = 0; i < e.num_obstaculos; i++)
		if(posicao_igual (e.obstaculo[i].posicao, x, y) && (e.obstaculo[i].state == 0))
			return 1;

	return 0;
}


int tem_jogador(ESTADO e, int x, int y){

	return posicao_igual (e.jog.posicao , x , y);

}

int posicao_ocupada (ESTADO e, int x, int y){

	return tem_inimigo(e, x, y) || tem_obstaculo(e, x, y) || tem_jogador(e, x, y);

}

ESTADO addpot(ESTADO e) {

	if (e.num_pots < 10)
		e = inicializar_pot(e);

	return e;
}

ESTADO apagaPOTS (ESTADO e, int identificador) {
	int i;

	for(i = identificador; i < e.num_pots; i++) {
		e.pocao[i] = e.pocao[i+1];
	}

	e.num_pots--;

	return e;
}


ESTADO modificaPOTS(ESTADO e) {
	int i, k;

	for(i=0; i < e.num_pots; i++)
		e.pocao[i].dur--;

	for(i=0; i < e.num_pots; i++) {
		if (e.pocao[i].dur < 0) {
			for(k=i; k < e.num_pots; k++)
				e.pocao[k] = e.pocao[k+1];
			e.num_pots--;
			i--;
		}
	}

	return e;
}

ESTADO apanhapot(ESTADO e, int x, int y) {
	int i, type;

	for (i = 0; (!posicao_igual(e.pocao[i].pos,x,y)); i++);

	type = e.pocao[i].type;

	if (type == 0) {
		e.jog.stats.vida += 1;

		if (e.jog.stats.vida > e.jog.stats.maxvida)
			e.jog.stats.vida = e.jog.stats.maxvida;

	}
	else if (type == 1) {
		e.jog.stats.mana += 3;

		if (e.jog.stats.mana > e.jog.stats.maxmana)
			e.jog.stats.mana = e.jog.stats.maxmana;

	}
	else if (type == 2) {
		e.jog.stats.energy += 1;

		if (e.jog.stats.energy > e.jog.stats.maxenergy)
			e.jog.stats.energy = e.jog.stats.maxenergy;

	}

	e = apagaPOTS(e,i);

	return e;
}
