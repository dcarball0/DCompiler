#include <stdio.h>
#include <string.h>

#include "symbols.h"
#include "defs.h"
#include "binTree.h"

/*
* table: arbol de busqueda binario para tabla de simbolos
* row: componentes reconocidos por el compilador
*/
binTree table;
lexComp symbols[] = {
	{"import", DIMPORT},
	{"identifier", DIDENTIFIER},
	{"double", DDOUBLE},
	{"string", DSTRING},
	{"int", DINT},
	{"float", DFLOAT},
	{"while", DWHILE},
	{"foreach", DFOREACH},
	{"return", DRETURN},
	{"void", DVOID},
	{"cast", DCAST},
	{"if", DIF}
};

/*
* Inicializar Tabla
*	Crea tabla
*	Inserta simbolos reconocidos por el compilador
*/
void initTable() {
	// Numero de simbolos
	int nSymbols = sizeof(symbols) / sizeof(symbols[0]);
	// Crear tabla simbolos
	treeCreate(&table);

	// Popular tabla simbolos
	for (int i = 0; i < nSymbols; i++)
		treeInsert(&table, symbols[i]);
}

/*
* Destruir Tabla
*	Libera memoria de la tabla
*/
void destroyTable() {
	treeDestroy(&table);
}

/*
* Recuperar id a partir de lexema
*	@param lex: Componente lexico del cual necesitamos rellenar el identificador
*/
void getID(lexComp* lex) {
	// Comprobamos si el lexema existe en la tabla
	if (treeIsMember(table, *lex)) {
		// Buscar nodo en la tabla de simbolos
		lexComp s;
		treeNodeSearch(table, lex->lex, &s);
		// Asignar 
		lex->id = s.id;
	}
}