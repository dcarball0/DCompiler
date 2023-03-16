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
lexComp row[] = {
	{"identificador", ID},
	{"import", IMPORT},
	{"string", STRING},
	{"int", INT},
	{"float", FLOAT},
	{"while", WHILE},
	{"foreach", FOREACH},
	{"return", RETURN},
	{"void", VOID},
	{"cast", CAST}
};

/*
* Inicializar Tabla
*	Crea tabla
*	Inserta simbolos reconocidos por el compilador
*/
void initTable() {
	treeCreate(table);
	for (int i = 0; i < 10; i++)
		treeInsert(table, row[i]);
}

/*
* Destruir Tabla
*	Libera memoria de la tabla
*/
void destroyTable() {
	treeDestroy(table);
}

