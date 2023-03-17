#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H

/* Lectura archivo */
#define ERR_FILE_OPEN 0001
#define ERR_FILE_READ 0002

/* Procesamiento lexema */
#define ERR_OVERFLOW_INTERNAL_BUFFER 1111

/* Errores sintaxis */
#define ERR_BAD_NUMBER 2221
#define ERR_UNCLOSED_STRING 2222


/*
* Manejador de errores
*	@param e: codigo de error
*/
void dCompError(int e);

#endif