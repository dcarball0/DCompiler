#include <stdlib.h>
#include "lexico.h"
#include "abb.h"

/*
* Analisis sintactico
*	Pide siguiente componente lexico
*	Imprime componente lexico
*	Libera componente lexico
*/
void syntaxAnalysis() {
	tipoelem c;

	while (nextLexComp(&c))
	{
		// imprimir codig lex

		if (c.lex) {
			free(c.lex);
			c.lex = NULL;
			c.id = 0;
		}
	}
}