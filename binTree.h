#ifndef BINTREE_H
#define BINTREE_H

typedef struct {
	char* lex;
	int id;
} lexComp;

typedef char* lexCompKey;

typedef struct celda * binTree; //tipo opaco

/////////////////////////////// FUNCIONES

//FUNCIONES DE CREACIÓN Y DESTRUCCIÓN DEL ÁRBOL
/**
 * Crea el arbol vacio.
 * @param A Puntero al arbol. Debe estar inicializado.
 */
void treeCreate(binTree *A);

/**
 * Destruye el arbol recursivamente
 * @param A El arbol que queremos destruir
 */
void treeDestroy(binTree *A);

//FUNCIONES DE INFORMACIÓN
/**
 * Comprueba si el arbol esta vacio
 * @param A El arbol binario
 */
unsigned treeIsEmpty(binTree A);

/**
 * Devuelve el subarbol izquierdo de A
 * @param A - Arbol original
 */
binTree treeL(binTree A);
/**
 * Devuelve el subarbol derecho de A
 * @param A - Arbol original
 */
binTree treeR(binTree A);

//FUNCIONES DE MODIFICACIÓN
/**
 * Inserta un nuevo nodo en el arbol para el elemento E
 * del que toma su clave. Esta clave no debe existir en
 * el arbol.
 * @param A Arbol binario
 * @param E Informacion del nuevo nodo.
 */
void treeInsert(binTree *A, lexComp E);

/**
 * Comprueba si el elemento <E> existe en el arbol <A>
 * @param A
 * @param E
 */
unsigned treeIsMember(binTree A, lexComp E);

/**
 * Busca un nodo con clave <cl> en el arbol y, si existe
 * modifica el puntero <nodo> para que lo apunte.
 * @param A Arbol binario en el que se busca el nodo.
 * @param cl Clave que se buscara.
 * @param nodo Puntero a un lexComp.
 */
void treeNodeSearch(binTree A, lexCompKey cl, lexComp* nodo);

/**
 * Imprime la tabla <A>
 * @param A tabla de simbolos
 */
void printTable(binTree A);

#endif	// binTree_H

