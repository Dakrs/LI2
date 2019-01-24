#include "animation.h"


void explosao (int x, int y, int identificador) {
	printf("<image id='burn%d' x='%d' y='%d' height='30' width='30' opacity='1' xlink:href='http://localhost/images/explosao1.png' />\n",identificador,x,y);
	printf("<animate xlink:href= #burn%d attributeType='auto' attributeName='opacity' to='0' dur='0.5s' begin='0s' fill='freeze' />\n",identificador);
}

void calPIXEL (int * x, int * y, POSICAO a,POSICAO b) {
	int x1, x2, y1, y2;

	x1 = a.x*42;
	x2 = b.x*42;

	if (a.x % 2)
		y1 = 48*a.y + 24 + a.y;
	else
		y1 = 48*a.y + a.y;

	if (b.x % 2)
		y2 = 48*b.y + 24+ b.y;
	else
		y2 = 48*b.y + b.y;

	(*y) = y2 - y1;
	(*x) = x2 - x1;
}

int quadrante (int x,int y) {
	int r = 0;

	if (x >= 0 && y >= 0)
		r = 1;
	else if (x <= 0 && y >= 0)
		r = 2;
	else if (x <= 0 && y <= 0)
		r = 3;
	else if (x >= 0 && y <= 0)
		r = 4;

	return r;
}

void calSingleCOORD (int x, int y, int * x1, int * y1) {
	int dist = x * 42;

	if (x % 2)
		(*y1) = 49*y+ 24;
	else
		(*y1) = 49*y;

	(*x1) = dist;

}


int calAGL (int x, int y) {
	double aux;
	int qua = quadrante(x,y);

	if (y == 0) {
		if (x >= 0)
			aux = 0;
		else
			aux = PI;
	}
	else {
		aux = atan(x/y);
		if (qua == 3 && aux >= 0)
			aux += PI;
		if (qua == 4 && aux <= 0)
			aux = 0.5*PI - aux;
	}

	aux = (aux * 360) / (2*PI);
	qua = (int) aux;

	return qua;
}

void imprime_armour(IFULL jog, int ajuste) {
	int x,y;

	calPIXEL(&x,&y,jog.ant,jog.posicao);
	if (jog.ant.x % 2) {
		IMAGEMSW((jog.ant.x * 42) + 9, (48 * jog.ant.y) + 24 + 13 + ajuste,1,17,24, "espada2f.png");
		ANIMATIONS(1,0,0,x,y);
		IMAGEMGE((jog.ant.x * 42) + 29, (48 * jog.ant.y) + 24 + 22 + ajuste,1,18,20, "escudo1f.png");
		ANIMATIONG(1,0,0,x,y);
	}
	else {
		IMAGEMSW((jog.ant.x * 42) + 9, (48 * jog.ant.y) + 13 + ajuste,1,17,24, "espada2f.png");
		ANIMATIONS(1,0,0,x,y);
		IMAGEMGE((jog.ant.x * 42) + 29, (48 * jog.ant.y)+ 22 + ajuste,1,18,20, "escudo1f.png");
		ANIMATIONG(1,0,0,x,y);
	}
}

void PRINT_IMG_ATK (int angulo, int x1, int y1) {
	int angulof = 360 - angulo;

	printf("<image id=jogATK x=%d y=%d width=40 height=54 opacity='1' transform='rotate(%d %d %d)' xlink:href='http://localhost/images/attack2f.png'/>\n",\
					x1 , y1 , angulof , x1+26 , y1+16);
}

void PRINT_ARROW(int angulo, int x, int y, int identificador) {
	int angulof = 360 - angulo;
	angulof += 135;

	if (angulof >= 360)
		angulof -= 360;

	printf("<image id=seta%d x=%d y=%d width=25 height=24 opacity='1' transform='rotate(%d %d %d)' xlink:href='http://localhost/images/arrowf.png' />\n",identificador,x+10,y+10,angulof,x+23,y+22);
}

void PRINT_ARCHER_ATK(POSICAO a,POSICAO b, int x , int y, int identificador) {
	int dx, dy;

	calPIXEL(&dx,&dy,a,b);
	PRINT_ARROW(calAGL(dx,dy),x,y,identificador);

	printf("<animateMotion xlink:href=#seta%d dur=1s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													identificador,0,0,dx,dy);

	printf("<animate xlink:href=#seta%d attributeType='auto' attributeName='opacity' to='0' dur='0.1s' begin='1s' fill='freeze' />\n",identificador);
}

/*
Função que desenha a animação do jogador a atacar.
*/

void PRINT_IMG_ATK_ALL(POSICAO a,POSICAO b, int x , int y) {
	int dx, dy;

	calPIXEL(&dx,&dy,a,b);

	dx *= 0.5;
	dy *= 0.5;

	PRINT_IMG_ATK(calAGL(dx,dy),x,y);
	ANIMATIONJATK(0,0,dx,dy);
	printf("<animate xlink:href= #jogATK attributeType='auto' attributeName='opacity' to='0' dur='0.1s' begin='0.4s' fill='freeze' />\n");
}

/*
Função que desenha um inimigo auxilar para o ataque com um timer de 0.25.
*/

void DRAW_ENEMY_AUX2 (int x,int y, int tipo) {
	if (tipo)
		printf("<image id=eneA x=%d y=%d width=45 height=38 opacity='1' xlink:href='http://localhost/images/mob1f.png'/>\n",x+6,y+8);
	else
		printf("<image id=eneA x=%d y=%d width=45 height=38 opacity='1' xlink:href='http://localhost/images/archer1f.png'/>\n",x+6,y+4);

	printf("<animate xlink:href= #eneA attributeType='auto' attributeName='opacity' to='0' dur='0.1s' begin='0.25s' fill='freeze' />\n");
}

void DRAW_ENEMY_AUX (int x,int y,int tipo) {
	if (tipo==1)
		printf("<image id=eneS x=%d y=%d width=45 height=38 opacity='1' xlink:href='http://localhost/images/mob1f.png'/>\n",x+6,y+8);
	else
		printf("<image id=eneS x=%d y=%d width=45 height=38 opacity='1' xlink:href='http://localhost/images/archer1f.png'/>\n",x+6,y+4);

	printf("<animate xlink:href= #eneS attributeType='auto' attributeName='opacity' to='0' dur='0.1s' begin='0.4s' fill='freeze' />\n");
}

void DRAW_LAST_PLAYER (int x,int y) {

	printf("<image id=jog x=%d y=%d width=%d height=%d opacity='0' xlink:href=%s />\n", \
												x, y, 45, 38, "http://localhost/images/zelda1f.png");

	printf("<animate xlink:href= #jog attributeType='auto' attributeName='opacity' to='1' dur='0.1s' begin='0.5s' fill='freeze' />\n");
}

void DRAW_GEAR_ANI(int x,int y) {

	printf("<image id=esp x=%d y=%d width=17 height=24 opacity='0' xlink:href='http://localhost/images/espada2f.png'/>\n",x+9,y+13);
	printf("<animate xlink:href= #esp attributeType='auto' attributeName='opacity' to='1' dur='0.1s' begin='0.5s' fill='freeze' />\n");
	printf("<image id=esc x=%d y=%d width=18 height=20 opacity='0' xlink:href='http://localhost/images/escudo1f.png'/>\n",x+29,y+22);
	printf("<animate xlink:href= #esc attributeType='auto' attributeName='opacity' to='1' dur='0.1s' begin='0.5s' fill='freeze' />\n");

}

void DRAW_VOID_SPELL(int x, int y, int z1, int z2) {

	printf("<image id=blabla x=%d y=%d width=30 height=30 transform='rotate(0 %d %d)' xlink:href='http://localhost/images/vortexSf.png'/>\n)",x+13,y+10,x+28,y+28);
	printf("<animateTransform xlink:href= #blabla attributeType='auto' attributeName='transform' type='rotate' from='0 %d %d' to='360 %d %d' dur='0.2s' repeatCount='indefinite' /> \n",x+28,y+28,x+28,y+28);
	printf("<animateMotion xlink:href= #blabla dur=0.25s begin=0s fill=freeze path='M0,0 %d,%d' />\n",z1,z2);
	printf("<animate xlink:href= #blabla attributeType='auto' attributeName='opacity' to='0' dur='0.1s' begin='0.25s' fill='freeze' />\n");

}

void DRAW_SPELL_FREEZE(int identificador, int x, int y) {
	calSingleCOORD(x,y,&x,&y);

	printf("<image id=obs%d x=%d y=%d width=%d height=%d opacity=0 xlink:href=%s />\n", \
						identificador,x, y, 56 , 49 , "http://localhost/images/freezef.png");
	printf("<animate xlink:href= #obs%d attributeType='auto' attributeName='opacity' to='1' dur='0.4s' begin='0s' fill='freeze' />\n",identificador);

}

void DRAW_FULL_ATK (ESTADO e) {
	int x, y;

	calSingleCOORD(e.auxATK.x,e.auxATK.y,&x,&y);
	DRAW_ENEMY_AUX(x,y,e.specatk);
	calSingleCOORD(e.jog.posicao.x,e.jog.posicao.y,&x,&y);
	PRINT_IMG_ATK_ALL(e.jog.posicao,e.auxATK,x+6,y+8);
	DRAW_LAST_PLAYER(x+6,y+8);
	DRAW_GEAR_ANI(x,y);

}
