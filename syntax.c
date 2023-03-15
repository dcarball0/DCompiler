#include <stdlib.h>
#include "lexer.h"
#include "binTree.h"

/*
* Analisis sintactico
*	Pide siguiente componente lexico
*	Imprime componente lexico
*	Libera componente lexico
*/
void syntaxAnalysis() {
    // Buffer Componente lexico
	lexComp c;

    // Mientras siga habiendo componentes lexicos
	while (nextLexComp(&c))
	{
		// Imprimir codigo lexico
		syntaxPrintLexComp(c);

        // Mandar a otro sitio

        // Liberar memoria una vez leido
		if (c.lex) {
			free(c.lex);
			c.lex = NULL;
			c.id = 0;
		}
	}
}

/*
* Imprime componente lexico <c>
* @param c: componente lexico a imprimir
*/
void syntaxPrintLexComp(lexComp c) {
	printf("<%d, ", c.id);
    if (printSpecialChar(c.lex) < 0)
        printf("%s> \n", c.lex);
    else
        printf("> \n", c.lex);
}

/*
* Imprime caracteres especiales
* @param _c: caracter a imprimir si es especial
* @return -1: si no es especial
* @return 0: si es especial
*/
int printSpecialChar(char* _c) {

    // Null check
    if (!_c) return -1;

    // Comprobamos el primer caracter solo
    switch (_c[0]) {
        case '\n':
            printf("\\n");
            break;
        case '\r':
            printf("\\r");
            break;
        case '\t':
            printf("\\t");
            break;
        default:
            if ((_c < 0x20) || (_c > 0x7f))
                printf("\\%03o", _c[0]);
            else
                return -1;
            break;
    }
    return 0;
}