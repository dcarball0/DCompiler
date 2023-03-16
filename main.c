#include "input.h"
#include "symbols.h"
#include "syntax.h"

int main(int argc, char** argv) {
	// Inicializar sistema de entrada
	dOpen("B:\\Users\\SeiGaX\\Desktop\\CompInter\\DCompiler\\regression.d");

	// Inicializar tabla de simbolos
	initTable();

	// Realizar analisis sintactico
	syntaxAnalysis();

	// Liberar recursos
	destroyTable();
	dClose();
	for (;;);
}