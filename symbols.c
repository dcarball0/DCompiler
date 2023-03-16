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
	{"cast", DCAST}
};

/*
* Inicializar Tabla
*	Crea tabla
*	Inserta simbolos reconocidos por el compilador
*/
void initTable() {
	int nRows = sizeof(symbols) / sizeof(symbols[0]);
	treeCreate(&table);

	for (int i = 0; i < nRows; i++)
		treeInsert(&table, symbols[i]);
}

/*
* Destruir Tabla
*	Libera memoria de la tabla
*/
void destroyTable() {
	treeDestroy(&table);
}

void getID(lexComp* lex) {
	if (treeIsMember(table, *lex)) {
		lexComp s;
		treeNodeSearch(table, lex->lex, &s);
		lex->id = s.id;
	}
}