#include "binTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////// ESTRUCTURAS DE DATOS

struct celda {
    lexComp info;
    struct celda *treeL, *treeR;
};

//////////////////////// FUNCIONES


/////////////////////////////////////////////////////////////
/////////////////////////// INICIO PARTE MODIFICABLE

/*Extraer la clave de una celda */
lexCompKey treeGetNodeKey(lexComp *E) {
    return E->lex;
}

/* Esta funcion puente nos permite modificar el tipo de
 * de datos del TAD sin tener que cambiar todas las 
 * comparaciones del resto de la biblioteca y en su lugar
 * cambiando solo esta. */
int treeCompareKeys(lexCompKey cl1, lexCompKey cl2) {
    return strcmp(cl1,cl2)==0 ? 0 : strcmp(cl1,cl2)>0 ? 1 : -1;
}

/////////////////////////// FIN PARTE MODIFICABLE
/////////////////////////////////////////////////////////////

//OPERACIONES DE CREACIÓN Y DESTRUCCIÓN

void treeCreate(binTree *A) {
    *A = NULL;
}

void treeDestroy(binTree *A) {
    if (*A != NULL) {
        treeDestroy(&(*A)->treeL);
        treeDestroy(&(*A)->treeR);
        if (&(*A)->info.lex != NULL) {
            free((*A)->info.lex);
            (*A)->info.lex = NULL;
        }
        free(*A);
        *A = NULL;
    }
}

void treeDestroyLexComp(lexComp *lex) {
    free(lex->lex);
}

//OPERACIONES DE INFORMACIÓN

unsigned treeIsEmpty(binTree A) {
    return A == NULL;
}

binTree treeL(binTree A) {
    return A->treeL;
}

binTree treeR(binTree A) {
    return A->treeR;
}

void treeRead(binTree A, lexComp *E) {
    *E = A->info;
}
// Función privada para comparar las claves

int _treeCompareKey(lexCompKey cl, lexComp E) {
    return treeCompareKeys(cl, treeGetNodeKey(&E));
}
//Función privada para informar si una clave está en el árbol

unsigned treeIsMemberByKey(binTree A, lexCompKey cl) {
    if (treeIsEmpty(A)) {
        return 0;
    }
    int comp = _treeCompareKey(cl, A->info);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    if (comp > 0) { //cl > A->info
        return treeIsMemberByKey(treeR(A), cl);
    }
    //cl < A->info
    return treeIsMemberByKey(treeL(A), cl);
}

//Funciones públicas

unsigned treeIsMember(binTree A, lexComp E) {
    return treeIsMemberByKey(A, treeGetNodeKey(&E));
}

void treeNodeSearch(binTree A, lexCompKey cl, lexComp *nodo) {
    if (treeIsEmpty(A)) {
        return;
    }
    int comp = _treeCompareKey(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    } else if (comp < 0) { // cl < A->info
        treeNodeSearch(A->treeL, cl, nodo);
    } else { // cl > A->info
        treeNodeSearch(A->treeR, cl, nodo);
    }
}
//OPERACIONES DE MODIFICACIÓN

/* Funcion recursiva para insertar un nuevo nodo 
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void treeInsert(binTree *A, lexComp E) {
    if (treeIsEmpty(*A)) {
        *A = (binTree) malloc(sizeof (struct celda));
        if (*A) {
            (*A)->info = E;
            (*A)->treeL = NULL;
            (*A)->treeR = NULL;
        }
        return;
    }
    lexCompKey cl = treeGetNodeKey(&E);
    int comp = _treeCompareKey(cl, (*A)->info);
    if (comp > 0) {
        treeInsert(&(*A)->treeR, E);
    } else {
        treeInsert(&(*A)->treeL, E);
    }
}

/* Funcion privada que devuelve mínimo de subárbol dcho */
lexComp treeRemoveNodeMin(binTree * A) {//Se devuelve el elemento más a la izquierda
    binTree aux;
    lexComp ele;
    if (treeIsEmpty((*A)->treeL)) {//Si izquierda vacía, se devuelve valor nodo actual A
        ele = (*A)->info;
        aux = *A;
        *A = (*A)->treeR;
        free(aux);
        return ele;
    } else {
        return treeRemoveNodeMin(&(*A)->treeL); //se vuelve a buscar mínimo rama izquierda
    }
}

/* Funcion que permite eliminar un nodo del arbol */
void treeRemoveNode(binTree *A, lexComp E) {
    binTree aux;
    if (treeIsEmpty(*A)) {
        return;
    }

    lexCompKey cl = treeGetNodeKey(&E);
    int comp = _treeCompareKey(cl, (*A)->info);
    if (comp < 0) { //if (E < (*A)->info) {
        treeRemoveNode(&(*A)->treeL, E);
    } else if (comp > 0) { //(E > (*A)->info) {
        treeRemoveNode(&(*A)->treeR, E);
    } else if (treeIsEmpty((*A)->treeL) && treeIsEmpty((*A)->treeR)) {
        treeDestroyLexComp(&((*A)->info));
        free(*A);
        *A = NULL;
    } else if (treeIsEmpty((*A)->treeL)) { // pero no es vacio derecha
        aux = *A;
        *A = (*A)->treeR;
        treeDestroyLexComp(&aux->info);
        free(aux);
    } else if (treeIsEmpty((*A)->treeR)) { //pero no es vacio izquierda
        aux = *A;
        *A = (*A)->treeL;
        treeDestroyLexComp(&aux->info);
        free(aux);
    } else { //ni derecha ni izquierda esta vacio, busco mínimo subárbol derecho
        treeDestroyLexComp(&(*A)->info); //elimino la info pero no libero el nodo,
        //pues en su sitio voy a poner el mínimo del subárbol derecho
        (*A)->info = treeRemoveNodeMin(&(*A)->treeR);
    }
}

/* Funcion privada para pasar la clave y no tener que
   extraerla del nodo en las llamadas recursivas.*/
void _treeModify(binTree A, lexCompKey cl, lexComp nodo) {
    if (treeIsEmpty(A)) {
        return;
    }
    int comp = _treeCompareKey(cl, A->info);
    if (comp == 0) {
        A->info = nodo;
    } else if (comp < 0) {
        _treeModify(A->treeL, cl, nodo);
    } else {
        _treeModify(A->treeR, cl, nodo);
    }
}


/* Permite modificar el nodo extrayendo del mismo la clave */
void treeModify(binTree A, lexComp nodo) {
    lexCompKey cl = treeGetNodeKey(&nodo);
    _treeModify(A, cl, nodo);
}
