#include "errorHandler.h"
#include "input.h"
#include <stdio.h>

/*
* Manejador de errores
*	@param e: codigo de error
*/
void dCompError(int e) {
	switch (e) {
		/* LECTURA ARCHIVO */
	case ERR_FILE_OPEN:
		printf("[IO%d] Error opening file.", e);
	case ERR_FILE_READ:
		printf("[IO%d] Error reading file.", e);

	/* Procesamiento lexema */
	case ERR_OVERFLOW_INTERNAL_BUFFER:
		printf("[C%d] An identifier exceeds the length of the internal buffer used for identifiers. Shorten the name.", e);
		break;

	/* SINTAXIS */
	case ERR_BAD_NUMBER:
		printf("[S%d] Malformed number.", e);
		break;
	case ERR_UNCLOSED_STRING:
		printf("[S%d] Unclosed string.", e);
		break;

	}
	printf("\n");
}