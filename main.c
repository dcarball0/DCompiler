#include "input.h"
#include "symbols.h"
#include "syntax.h"

int main(int argc, char** argv) {
	// Comprobar numero adecuado de argumentos
	if (argc == 2) 
	{
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
		//for (;;);
	}
	else 
	{
		printf("Uso ./dCompiler [archivo]\n");
	}
}