#ifndef ___CGI_H___
#define ___CGI_H___

/**
@file cgi.h
Macros úteis para gerar CGIs
*/

#include <stdio.h>

/**
* Caminho para as imagens
*/
#define IMAGE_PATH							"http://localhost/images/"

/**
\brief Macro para abrir o style.
*/

#define STYLEO printf("<style>\n");

/**
\brief Macro para fechar o style.
*/

#define STYLEC printf("</style>\n");

/**
\brief Macro para dar import a um cursor para a pagina.
*/

#define CURSOR printf(" a {cursor:url(%s), auto;}\n","http://localhost/images/cursorf.png");

/**
\brief Macro para dar import a um font de texto diferente.
*/

#define STYLE printf("@font-face { font-family: VT323; src: url(http://localhost/images/VT323-Regular.ttf);\n");

/**
\brief Macro para gerar texto.
@param texto Texto que será escrito.
@param size Tamanho da letra.
@param x Coordenada x da página.
@param y Coordenada y da página.
@param cor Cor do texto.
*/
#define TEXT(texto,size,x,y,cor)		 printf("<text x=%d y=%d font-family = 'VT323' font-size=%d fill=%s > %s </text>",x,y,size,cor,texto);



/**
\brief Macro para começar o html
*/
#define COMECAR_HTML						printf("Content-Type: text/html\n\n")

/**
\brief Macro para abrir um svg
@param tamx O comprimento do svg
@param tamy A altura do svg
*/
#define ABRIR_SVG(tamx, tamy)				printf("<svg width=%d height=%d>\n", tamx, tamy)
/**
\brief Macro para fechar um svg
*/
#define FECHAR_SVG							printf("</svg>\n")

/**
\brief Macro para criar um retangulo.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param COR Cor do retangulo.
*/

#define LIFEBAR(X, Y, LARGURA, ALTURA, COR) printf("<rect x=%d y=%d width=%d height=%d fill=%s />\n",X ,Y ,LARGURA ,ALTURA , COR)

/**
\brief Macro Imprimir o jogador.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMJJ(X, Y, LARGURA ,ALTURA , FICHEIRO)		printf("<image id=jog x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												X, Y, LARGURA, ALTURA, IMAGE_PATH FICHEIRO)

/**
\brief Macro Imprimir o escudo do jogador.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param IN Valor do identificador da imagem.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMGE(X, Y, IN, LARGURA ,ALTURA , FICHEIRO)		printf("<image id=gear%d x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												IN,X, Y, LARGURA, ALTURA, IMAGE_PATH FICHEIRO)

/**
\brief Macro Imprimir a espada do jogador.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param IN Valor do identificador da imagem.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMSW(X, Y, IN, LARGURA ,ALTURA , FICHEIRO)		printf("<image id=sword%d x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												IN,X, Y, LARGURA, ALTURA, IMAGE_PATH FICHEIRO)

/**
\brief Macro Imprimir uma imagem..
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMSS(X, Y, LARGURA ,ALTURA , FICHEIRO)		printf("<image x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												X, Y, LARGURA, ALTURA, IMAGE_PATH FICHEIRO)

/**
\brief Macro Imprimir um obstaculo.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param IN Valor do identificador da imagem.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMOO(X, Y, IN, LARGURA ,ALTURA , FICHEIRO)		printf("<image id=obs%d x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												IN,X, Y, LARGURA, ALTURA, IMAGE_PATH FICHEIRO)

/**
\brief Macro Imprimir um inimigo.
@param X A coordenada X do canto superior esquerdo.
@param Y A coordenada Y do canto superior esquerdo.
@param IN Valor do identificador da imagem.
@param LARGURA Largura do retangulo.
@param ALTURA Altura do retangulo.
@param FICHEIRO Local da imagem.
*/

#define IMAGEMEE(X, Y, IN, LARGURA ,ALTURA , FICHEIRO)		printf("<image id=ene%d x=%d y=%d width=%d height=%d xlink:href=%s />\n", \
												IN, X, Y , LARGURA, ALTURA, IMAGE_PATH FICHEIRO)


/**
\brief Macro Imprimir uma animação para mover um jogador.
@param X1 A coordenada inicial dos x.
@param Y1 A coordenada inicial dos y.
@param X2 Deslocação no eixo dos x.
@param Y2 Deslocação no eixo dos y.
*/

#define ANIMATIONJ(X1,Y1,X2,Y2) printf("<animateMotion xlink:href= #jog dur=0.5s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													X1,Y1,X2,Y2);

/**
\brief Macro Imprimir uma animação para mover uma espada.
@param i Identificador da animação.
@param X1 A coordenada inicial dos x.
@param Y1 A coordenada inicial dos y.
@param X2 Deslocação no eixo dos x.
@param Y2 Deslocação no eixo dos y.
*/

#define ANIMATIONS(i,X1,Y1,X2,Y2) printf("<animateMotion xlink:href= #sword%d dur=0.5s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													i,X1,Y1,X2,Y2);

/**
\brief Macro Imprimir uma animação para mover um escudo.
@param i Identificador da animação.
@param X1 A coordenada inicial dos x.
@param Y1 A coordenada inicial dos y.
@param X2 Deslocação no eixo dos x.
@param Y2 Deslocação no eixo dos y.
*/

#define ANIMATIONG(i,X1,Y1,X2,Y2) printf("<animateMotion xlink:href= #gear%d dur=0.5s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													i,X1,Y1,X2,Y2);

/**
\brief Macro Imprimir uma animação para mover um inimigo.
@param i Identificador da animação.
@param X1 A coordenada inicial dos x.
@param Y1 A coordenada inicial dos y.
@param X2 Deslocação no eixo dos x.
@param Y2 Deslocação no eixo dos y.
*/

#define ANIMATIONE(i,X1,Y1,X2,Y2) printf("<animateMotion xlink:href= #ene%d dur=0.5s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													i,X1,Y1,X2,Y2);

/**
\brief Macro Imprimir uma animação para mover o ataque do jogador.
@param X1 A coordenada inicial dos x.
@param Y1 A coordenada inicial dos y.
@param X2 Deslocação no eixo dos x.
@param Y2 Deslocação no eixo dos y.
*/

#define ANIMATIONJATK(X1,Y1,X2,Y2) printf("<animateMotion xlink:href= #jogATK dur=0.4s begin=0s fill=freeze path='M%d,%d %d,%d' />\n", \
													X1,Y1,X2,Y2);


/**
\brief Macro para abrir um link.
@param link O caminho para o link.
*/
#define ABRIR_LINK(link)					printf("<a xlink:href=%s>\n", link)


/**
\brief Macro para abrir um link
@param link O caminho para o link.
@param link2 Indentificador do link.
*/

#define ABRIR_LINK2(link,link2)					printf("<a xlink:href=%s id=%s>\n", link,link2)

/**
\brief Macro para fechar o link
*/
#define FECHAR_LINK							printf("</a>\n")

#endif
