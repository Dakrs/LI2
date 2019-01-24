#include "pathfinder.h"

int posicao_valida (int x, int y) {

	return x >= 0 && y >= 0 && x < TAM && y < TAM2;

}

static int posicao_igual(POSICAO p, int x, int y){

	return p.x == x && p.y == y;

}


int validaMovHexPar(int x,int y) {
	if (x==1 && y==1)
		return 0;
	if (x==-1 && y == 1)
		return 0;

	return 1;
}


int validaMovHexImp(int x,int y) {
	if (x==-1 && y==-1)
		return 0;
	if (x==1 && y ==-1)
		return 0;

	return 1;
}

int validaTudo(int coordX,int x, int y) {
	if (coordX%2)
		return validaMovHexImp(x,y);
	else
		return validaMovHexPar(x,y);
}

int verificaATK (ESTADO e, int x, int y);

MATRIZ preencher(MATRIZ mat , int x, int y) {
	int dx,dy;

	if(posicao_valida(x,y) == 0)
		return mat;

	for(dy=-1; dy <= 1; dy++)
		for(dx = -1; dx <= 1 ; dx++)
			if (dx != 0 || dy != 0) {
				if (posicao_valida(x+dx,y+dy) && (mat->map[y+dy][x+dx] > mat->map[y][x] + 1) && (mat->map[y][x] < 15)) {
					mat->map[y+dy][x+dx]= mat->map[y][x] + 1;
					mat = preencher(mat,x+dx,y+dy);
				}
			}

	return mat;
}


MATRIZ pathfinding (ESTADO e) {
	int i, k;
	MATRIZ mat;

	mat = malloc(sizeof(struct matriz));

	for(i=0; i <  e.num_obstaculos; i++)
		mat->map[ e.obstaculo[i].posicao.y][ e.obstaculo[i].posicao.x] = -2;

	for(i=0; i <  e.num_inimigos; i++)
		mat->map[ e.inimigo[i].posicao.y][ e.inimigo[i].posicao.x] = -2;

	for(i=0; i < TAM2; i++)
		for(k=0; k < TAM; k++)
			if (mat->map[i][k] != -2)
				mat->map[i][k] = 999;

	mat->map[e.jog.posicao.y][e.jog.posicao.x] = 0;
	mat = preencher(mat,e.jog.posicao.x,e.jog.posicao.y);

	return mat;

}

int checkMove (ESTADO e, int num_inimigo) {
	int dx , dy;
	int k =0;
	POSICAO v1 [10];

	for(dy = -1; dy <= 1; dy++){
		for(dx = -1; dx <= 1; dx ++) {
			if (dx != 0 || dy != 0) {
				if (validaTudo(e.inimigo[num_inimigo].posicao.x,dx,dy) && posicao_valida(e.inimigo[num_inimigo].posicao.x + dx,e.inimigo[num_inimigo].posicao.y + dy)) {
					v1[k].x = e.inimigo[num_inimigo].posicao.x + dx;
					v1[k++].y = e.inimigo[num_inimigo].posicao.y + dy;
				}
			}
		}
	}

	for(k=0; k < 6; k++)
		if (posicao_igual(e.jog.posicao,v1[k].x,v1[k].y))
			return 0;

	return 1;
}

ESTADO mobArcherXY (ESTADO e, int num_inimigo, MATRIZ * mat) {
	int dx, dy, x, y, r, t1, t2;

	r = 0;
	t1 = 0;
	t2 = 0;

	x = e.inimigo[num_inimigo].posicao.x;
	y = e.inimigo[num_inimigo].posicao.y;

	for(dy = -1; dy <= 1; dy++) {
		for(dx = -1; dx <= 1; dx++) {
			if (dx != 0 || dy != 0) {
				if (posicao_valida(x+dx,y+dy) && (r != 0) && ((*mat)->map[y+dy][x+dx] > 0) && validaTudo(x,dx,dy) && verificaATK(e,x+dx,y+dy)) {
					if ((*mat)->map[y+dy][x+dx] > r) {
						t1 = x+dx;
						t2 = y+dy;
						r = (*mat)->map[t2][t1];
					}
				}
				else if (posicao_valida(x+dx,y+dy) && ((*mat)->map[y+dy][x+dx] > 0) && validaTudo(x,dx,dy) && verificaATK(e,x+dx,y+dy)) {
					t1 = x+dx;
					t2 = y+dy;
					r = (*mat) -> map[t2][t1];
				}
			}
		}
	}

	if (r != 0) {
		e.inimigo[num_inimigo].ant = e.inimigo[num_inimigo].posicao;
		e.inimigo[num_inimigo].posicao.x = t1;
		e.inimigo[num_inimigo].posicao.y = t2;
	}

	return e;
}

ESTADO mobMoveXY (ESTADO e, int num_inimigo, MATRIZ * mat) {
	int dx, dy, x, y, r, t1, t2;

	r = 0;
	t1 = 0;
	t2 = 0;

	x = e.inimigo[num_inimigo].posicao.x;
	y = e.inimigo[num_inimigo].posicao.y;

	for(dy = -1; dy <= 1; dy ++){
		for(dx = -1; dx <= 1; dx++) {
			if (dx != 0 || dy != 0) {
				if (posicao_valida(x+dx,y+dy) && (r != 0) && ((*mat)->map[y+dy][x+dx] > 0) && validaTudo(x,dx,dy)) {
					if((*mat)->map[y+dy][x+dx] < r) {
						t1 = x+dx;
						t2 = y+dy;
						r = (*mat)->map[t2][t1];
					}
				}
				else if (posicao_valida(x+dx,y+dy) && ((*mat)->map[y+dy][x+dx] > 0) && validaTudo(x,dx,dy)) {
					t1 = x + dx;
					t2 = y +dy;
					r =(*mat)->map[t2][t1];
				}
			}
		}
	}

	if (r != 0) {
		e.inimigo[num_inimigo].ant = e.inimigo[num_inimigo].posicao;
		e.inimigo[num_inimigo].posicao.x = t1;
		e.inimigo[num_inimigo].posicao.y = t2;
	}

	return e;
}

static int tem_inimigo (ESTADO e, int x, int y){
		int i;

		for(i = 0; i < e.num_inimigos; i++)
			if(posicao_igual (e.inimigo[i].posicao, x, y))
				return 1;

		return 0;
}

static int tem_jogador(ESTADO e, int x, int y){

	return posicao_igual (e.jog.posicao , x , y);

}


int coordUP_RIGHT (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2)
      x += 1;
    else {
      x += 1;
      y -= 1;
    }

    if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
	}
	else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return 0;
    		if (tem_jogador(e,x,y))
    			return 1;
   	 	}
   	}
  }
  return 0;
}

int coordDOWN_RIGHT (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x += 1;
      y += 1;
    }
    else
      x += 1;

  	if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
	}
	else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return 0;
    		if (tem_jogador(e,x,y))
    			return 1;
   	 	}
   	}
  }
  return 0;
}

int coordUP_LEFT (ESTADO e, int x, int y) {
  int i;

  for(i = 0; i < 4; i++) {
    if (x % 2)
      x -= 1;
    else {
      x -= 1;
      y -= 1;
    }

    if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
	}
	else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return 0;
    		if (tem_jogador(e,x,y))
    			return 1;
    	}
    }
  }
  return 0;
}

int coordDOWN_LEFT (ESTADO e, int x, int y) {
  int i;
  for(i = 0; i < 4; i++) {
    if (x % 2) {
      x -= 1;
      y += 1;
    }
    else
      x -= 1;

  	if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
	}
	else {
    	if (posicao_valida(x,y)) {
    		if (tem_inimigo(e,x,y))
    			return 0;
    		if (tem_jogador(e,x,y))
    			return 1;
    	}
    }
  }
  return 0;
}

int coordUP (ESTADO e, int x, int y) {
	int i;

	for(i=0; i < 4; i++) {
		y += 1;

		if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
		}
		else {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
    				return 0;
    			if (tem_jogador(e,x,y))
    			return 1;
    		}
    	}
	}
	return 0;
}

int coordDOWN (ESTADO e, int x, int y) {
	int i;

	for(i=0; i < 4; i++) {
		y -= 1;

		if (i == 0) {
			if (posicao_valida(x,y)) {
				if (tem_inimigo(e,x,y))
					return 0;
			}
		}
		else {
			if (posicao_valida(x,y)) {
    			if (tem_inimigo(e,x,y))
    				return 0;
    			if (tem_jogador(e,x,y))
    				return 1;
    		}
    	}
	}
	return 0;
}

int verificaATK (ESTADO e, int x, int y) {

	return coordDOWN(e,x,y) || coordUP(e,x,y) || coordDOWN_LEFT(e,x,y) || coordUP_LEFT(e,x,y) || coordDOWN_RIGHT(e,x,y) || coordUP_RIGHT(e,x,y);

}
