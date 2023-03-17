#include "input.h"
#include "symbols.h"
#include "syntax.h"

int main(int argc, char** argv) {
	// Inicializar sistema de entrada
	dOpen(argv[1]);

	// Inicializar tabla de simbolos
	initTable();

	// Realizar analisis sintactico
	syntaxAnalysis();

	// Liberar recursos
	destroyTable();
	dClose();
	// Espera infinita
	for (;;);
}