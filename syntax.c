#include <stdlib.h>
#include <stdio.h>
#include "lexer.h"
#include "binTree.h"
#include "syntax.h"

/*
* Analisis sintactico
*	Pide siguiente componente lexico
*	Imprime componente lexico
*	Libera componente lexico
*/
void syntaxAnalysis() {
    // Buffer Componente lexico
	lexComp comp;

    // Mientras siga habiendo componentes lexicos
	while (nextLexComp(&comp))
	{
		// Imprimir codigo lexico
		syntaxPrintLexComp(comp);

        // Mandar a otro sitio

        // Liberar memoria una vez leido
		if (comp.lex) {
			free(comp.lex);
			comp.lex = NULL;
			comp.id = 0;
		}
	}
}

/*
* Imprime componente lexico <c>
* @param c: componente lexico a imprimir
*/
void syntaxPrintLexComp(lexComp comp) {
	printf("<%d, ", comp.id);
    if (printSpecialChar(comp.lex) < 0)
        printf("%s> \n", comp.lex);
    else
        printf("> \n");
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
            return -1;
            break;
    }
    return 0;
}