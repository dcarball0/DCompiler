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
			comp.id = -1;
		}
	}
}

/*
* Imprime componente lexico <c>
*   @param c: componente lexico a imprimir
*/
void syntaxPrintLexComp(lexComp comp) {
	printf("<%d,%s> \n", comp.id, comp.lex);
}