#include "menu.h"

void startgame (int tipo, char * str, int x, int y) {
	char link [MAX_BUFFER];

	if (tipo)
		sprintf(link,"http://localhost/cgi-bin/perihelion?st1");
	else
		sprintf(link,"http://localhost/cgi-bin/perihelion?st3");

	ABRIR_LINK(link);
	TEXT(str,45,x,y,"#ffffff");
	FECHAR_LINK;
}

void menuDraw () {

	ABRIR_SVG(1000,625);
	IMAGEMSS(0,-80,1000,625,"menu1.jpg");
	printf("<text x=300 y=200 font-family = 'VT323' font-size=100 fill='#ffffff' > Perihelion </text>");
	startgame(0,"Start game",405,320);
	startgame(1,"Scoreboard",405,360);
	FECHAR_SVG;
}

void gameOverDraw (int score) {
	char link [MAX_BUFFER];
	char link2 [MAX_BUFFER];

	ABRIR_SVG(1000,625);
	sprintf(link2,"Your Score : %d", score);
	IMAGEMSS(0,0,1000,625,"gameover.png");
	printf("<text x=365 y=450 font-family = 'VT323' font-size=50 fill='#ffffff' > %s </text>",link2);

	sprintf(link,"http://localhost/cgi-bin/perihelion?aaa");

	ABRIR_LINK(link);

	printf("<rect x=307 y=360 width=375 height=40 opacity='0' fill='#ffffff' />\n");

	FECHAR_LINK;

	FECHAR_SVG;
}

void KEYBOARD_MOVES () {
	printf("<script>\n \
		window.addEventListener('keydown', checkKepPress, false);\n \
		function checkKepPress(e) { \n \
			if (e.keyCode == 65) \n \
				window.location.href = document.getElementById('x').href.animVal;\n \
			if (e.keyCode == 81) \n \
				window.location.href = document.getElementById('y').href.animVal;\n \
			if (e.keyCode == 87) \n \
				window.location.href = document.getElementById('u').href.animVal;\n");

	printf("			if (e.keyCode == 83) \n \
				window.location.href = document.getElementById('d').href.animVal;\n \
			if (e.keyCode == 69) \n \
				window.location.href = document.getElementById('z').href.animVal;\n \
			if (e.keyCode == 68) \n \
				window.location.href = document.getElementById('k').href.animVal;\n \
			if (e.keyCode == 13) { \n \
				e.preventDefault(); \n \
				document.getElementById('bot').onclick(); \n \
			} \n \
			\n \
		}\n \
</script>\n");
}

void startHTML () {
	COMECAR_HTML;
	printf("<meta charset='utf-8'>\n");
	STYLEO;
	STYLE;
	CURSOR;
	STYLEC;
	KEYBOARD_MOVES();
}

void imprime_AJUDAS_GUIDES () {
	TEXT("TIPS",60,650,220,"#ffffff");
	TEXT("1. Para ativar a single guide",25,650,270,"#ffffff");
	TEXT("é necessário carregar",25,680,300,"#ffffff");
	TEXT("no inimigo pretendido",25,680,330,"#ffffff");
	TEXT("depois da ativação da guide",25,680,360,"#ffffff");
	TEXT("2. Ambas as guides precisão",25,650,390,"#ffffff");
	TEXT("de ser desativadas depois",25,680,420,"#ffffff");
	TEXT("de utilizadas",25,680,450,"#ffffff");
	TEXT("3. E aconselhável dar",25,650,480,"#ffffff");
	TEXT("prioridade a armadura",25,680,510,"#ffffff");
	TEXT("4. Existem 3 spells para descobrir",25,650,540,"#ffffff");

}

void javascript2 () {
	printf("<script type='text/javascript' src='http://localhost/images/jstr.js'> </script>\n");
}

void imprime_LABEL_INPUT() {
	printf("<form>\n \
	<input id='ts' type='text' name='name' style='height: 36px; width: 180px; font-family: VT323, monospace; font-size: 36px; position: absolute; left: 400px; top: 300px;' maxlength='6'/>\n \
	<br/>\n \
	<input id='bot' type='button' value='Jogar' style='height: 30px; width: 60px; position: absolute; left: 460px; top: 350px;' onclick='changevalue();' />\n \
	</form>");
}

void imprime_AJUDA_KEYBINDS () {
	TEXT("KEYBINDS",60,120,220,"#ffffff");
	IMAGEMSS(110,250,200,200,"celulaf.png");
	TEXT("A",50,153,390,"#000000");
	TEXT("Q",50,153,330,"#000000");
	TEXT("E",50,255,330,"#000000");
	TEXT("D",50,255,390,"#000000");
	TEXT("S",50,204,422,"#000000");
	TEXT("W",50,204,302,"#000000");
}

void imprime_PAGINA_AJUDAS () {
	ABRIR_SVG(1000,600);
	LIFEBAR(0,0,1000,600,"#000000");
	imprime_AJUDA_KEYBINDS();
	imprime_AJUDAS_GUIDES();
	TEXT("USERNAME",60,390,220,"#ffffff");
	FECHAR_SVG;
	javascript2();
	imprime_LABEL_INPUT();

}

void draw_TREASURE_MENU (ESTADO e) {
	char link[MAX_BUFFER];

	ABRIR_SVG(800,500);

	sprintf(link,"Armour lvl: %d",e.jog.armour.shield);
	IMAGEMSS(0,0,800,500,"battlegroundf.png");
	TEXT("Spells Learnt:",45,490,40,"#000000");

	if (e.jog.armour.voidf)
		TEXT("Void Cast",30,490,80,"#000000");
	if (e.jog.armour.firef)
		TEXT("Burn",30,490,120,"#000000");
	if (e.jog.armour.frostf)
		TEXT("Freeze",30,490,160,"#000000");

	printf("<text x=%d y=%d font-family = 'VT323' font-size=%d fill=%s > %s </text>",490,200,30,"#000000",link);
	sprintf(link,"http://localhost/cgi-bin/perihelion?vid");
	ABRIR_LINK(link);
	IMAGEMSS(45,380,80,80,"fullhealf.png");
	FECHAR_LINK;
	TEXT("+HP",20,67,475,"#000000");

	if (e.jog.armour.voidf != 1) {
		sprintf(link,"http://localhost/cgi-bin/perihelion?vsf");
		ABRIR_LINK(link);
		IMAGEMSS(140,380,80,80,"voidstaf.png");
		FECHAR_LINK;
		TEXT("Void Spell",25,140,475,"#000000");
	}
	if (e.jog.armour.frostf != 1) {
		sprintf(link,"http://localhost/cgi-bin/perihelion?fst");
		ABRIR_LINK(link);
		IMAGEMSS(235,380,80,80,"froststaf.png");
		FECHAR_LINK;
		TEXT("Freeze Spell",20,250,475,"#000000");
	}
	if (e.jog.armour.firef != 1) {
		sprintf(link,"http://localhost/cgi-bin/perihelion?isf");
		ABRIR_LINK(link);
		IMAGEMSS(330,380,80,80,"firestaf.png");
		FECHAR_LINK;
		TEXT("Burn Spell",20,350,475,"#000000");
	}
	if (e.jog.armour.shield < 3) {
	sprintf(link,"http://localhost/cgi-bin/perihelion?sss");
	ABRIR_LINK(link);
	IMAGEMSS(425,380,80,80,"armourf.png");
	FECHAR_LINK;
	TEXT("+Armour",20,440,475,"#000000");
	}

	sprintf(link,"http://localhost/cgi-bin/perihelion?ddj");
	ABRIR_LINK(link);
	IMAGEMSS(520,380,80,80,"doublejumpf.png");
	FECHAR_LINK;
	TEXT("+Energy",20,530,475,"#000000");
	sprintf(link,"http://localhost/cgi-bin/perihelion?fff");
	ABRIR_LINK(link);
	IMAGEMSS(615,380,80,80,"freezeSff.png");
	FECHAR_LINK;
	TEXT("+Mana",20,630,475,"#000000");
	FECHAR_SVG;
}
