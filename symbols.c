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
	{"double", DDOUBLE},
	{"string", DSTRING},
	{"int", DINT},
	{"float", DFLOAT},
	{"while", DWHILE},
	{"foreach", DFOREACH},
	{"return", DRETURN},
	{"void", DVOID},
	{"cast", DCAST},
	/* keywords que no estan en regresion.d */
	{"if", DIF},
	{"abstract", DDATATYPE},
	{"alias", DRESERVED},
	{"asm", DRESERVED},
	{"assert", DRESERVED},
	{"auto", DDATATYPE},
	{"bool", DDATATYPE},
	{"break", DRESERVED},
	{"byte", DDATATYPE},
	{"case", DRESERVED},
	{"catch", DRESERVED},
	{"char", DDATATYPE},
	{"class", DDATATYPE},
	{"const", DRESERVED},
	{"continue", DRESERVED},
	{"dchar", DDATATYPE},
	{"debug", DRESERVED},
	{"default", DRESERVED},
	{"delegate", DRESERVED},
	{"deprecated", DRESERVED},
	{"else", DRESERVED},
	{"enum", DDATATYPE},
	{"export", DRESERVED},
	{"extern", DRESERVED},
	{"false", DRESERVED},
	{"final", DRESERVED},
	{"finally", DRESERVED},
	{"for", DRESERVED},
	{"foreach_reverse", DRESERVED},
	{"function", DRESERVED},
	{"goto", DRESERVED},
	{"immutable", DRESERVED},
	{"in", DRESERVED},
	{"inout", DRESERVED},
	{"interface", DRESERVED},
	{"invariant", DRESERVED},
	{"is", DRESERVED},
	{"lazy", DRESERVED},
	{"long", DDATATYPE},
	{"macro", DRESERVED},
	{"mixin", DRESERVED},
	{"module", DRESERVED},
	{"new", DRESERVED},
	{"nothrow", DRESERVED},
	{"null", DRESERVED},
	{"out", DRESERVED},
	{"override", DRESERVED},
	{"package", DRESERVED},
	{"pragma", DRESERVED},
	{"private", DRESERVED},
	{"protected", DRESERVED},
	{"public", DRESERVED},
	{"pure", DRESERVED},
	{"real", DDATATYPE},
	{"ref", DRESERVED},
	{"scope", DRESERVED},
	{"shared", DRESERVED},
	{"short", DRESERVED},
	{"static", DRESERVED},
	{"struct", DRESERVED},
	{"super", DRESERVED},
	{"switch", DRESERVED},
	{"synchronized", DRESERVED},
	{"template", DRESERVED},
	{"this", DRESERVED},
	{"throw", DRESERVED},
	{"true", DRESERVED},
	{"try", DRESERVED},
	{"typeid", DRESERVED},
	{"typeof", DRESERVED},
	{"ubyte", DDATATYPE},
	{"uint", DDATATYPE},
	{"ulong", DDATATYPE},
	{"union", DRESERVED},
	{"unittest", DRESERVED},
	{"ushort", DDATATYPE},
	{"version", DRESERVED},
	{"wchar", DDATATYPE},
	{"with", DRESERVED},

};

/*
* Inicializar Tabla
*	Crea tabla
*	Inserta simbolos reconocidos por el compilador
*/
void initTable() {
	// Numero de simbolos en la tabla precargada
	int nSymbols;
	nSymbols = sizeof(symbols) / sizeof(symbols[0]);
	// Crear tabla simbolos
	treeCreate(&table);

	// Popular tabla simbolos
	for (int i = 0; i < nSymbols; i++)
		treeInsert(&table, symbols[i]);

	// Imprimir tabla de simbolos
	printTable(table);
}

/*
* Destruir Tabla
*	Libera memoria de la tabla
*/
void destroyTable() {
	// Imprimir tabla de simbolos
	printTable(table);

	treeDestroy(&table);
}

/*
* Recuperar id a partir de lexema e insertar en tabla
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
	// Si no esta en tabla insertarlo
	else {
		lex->id = DIDENTIFIER;
		treeInsert(&table, *lex);
	}
}