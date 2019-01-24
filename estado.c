#include "estado.h"

#define MAX_BUFFER		10240

ESTADO str2estadoB (FILE * fp) {
	ESTADO e;

	e.byte=fread(&e, sizeof(ESTADO),1, fp);
	return e;
}

void estado2strB (ESTADO e, FILE * fp) {
	fwrite(&e, sizeof(ESTADO),1,fp);
}
