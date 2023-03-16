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
        // Asignar memoria a la celda
        *A = (binTree) malloc(sizeof (struct celda));

        //Asignar memoria al lexema y \0 y copiar datos
        (*A)->info.lex = (char*)malloc(sizeof(char) * (strlen(E.lex) + 1));
        strcpy((*A)->info.lex, E.lex);

        if (*A) {
            // Poner fin de string
            (*A)->info.lex[strlen(E.lex)] = '\0';
            (*A)->info.id = E.id;
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