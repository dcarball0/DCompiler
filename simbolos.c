#include "simbolos.h"
#include "definiciones.h"
#include "abb.h"

/*
* table: arbol de busqueda binario para tabla de simbolos
* row: componentes reconocidos por el compilador
*/
abb table;
tipoelem row[] = {
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
	crear(&table);

	for (int i = 0; i < 10; i++)
		insertSymbol(row[i]);
}

/*
* Destruir Tabla
*	Libera memoria de la tabla
*/
void destroyTable() {
	destruir(table);
}

/*
* Insertar row en abb table
*	@param row: elemento a añadir en la tabla
*/
void insertSymbol(tipoelem row) {
	//Simbolo a crear
	tipoelem s;

	// Reservar memoria y copiar informacion lexema
	s.lex = (char*)malloc(sizeof(char) * (strlen(row.lex) + 1));
	strcpy(s.lex, row.lex);

	// Asignar id
	s.id = row.id;

	// Insertar en tabla
	insert(&table, s);
}

