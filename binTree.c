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
// Función privada para comparar las claves
int _treeCompareKey(lexCompKey cl, lexComp E) {
    return treeCompareKeys(cl, treeGetNodeKey(&E));
}

//Funciones públicas
//OPERACIONES DE MODIFICACIÓN

/* Funcion recursiva para insertar un nuevo nodo 
   en el arbol. Se presupone que no existe un nodo
   con la misma clave en el arbol. */
void treeInsert(binTree *A, lexComp E) {
    if (treeIsEmpty(*A)) {
        // Asignar memoria a la celda
        *A = (binTree) malloc(sizeof (struct celda));

        if (*A) {
            //Asignar memoria al lexema y \0 y copiar datos
            (*A)->info.lex = (char*)malloc(sizeof(char) * (strlen(E.lex) + 1));
            if ((*A)->info.lex)
            {
                strcpy((*A)->info.lex, E.lex);
            }
            // Poner fin de string
            if ((*A)->info.lex)
            {
                (*A)->info.lex[strlen(E.lex)] = '\0';
            }
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

unsigned treeIsMember(binTree A, lexComp E) {
    return treeIsMemberByLex(A, treeGetNodeKey(&E));
}

void treeNodeSearch(binTree A, lexCompKey cl, lexComp* nodo) {
    if (treeIsEmpty(A)) {
        return;
    }
    int comp = _treeCompareKey(cl, A->info);

    if (comp == 0) { // cl == A->info
        *nodo = A->info;
    }
    else if (comp < 0) { // cl < A->info
        treeNodeSearch(A->treeL, cl, nodo);
    }
    else { // cl > A->info
        treeNodeSearch(A->treeR, cl, nodo);
    }
}

/*
 * Funcion encargada de encontrar un lexema dentro de el arbol. 
 * Si el elemento no esta en la tabla, se introducirá
 */
unsigned treeIsMemberByLex(binTree A, lexCompKey cl) {
    if (treeIsEmpty(A)) {
        return 0;
    }
    
    int comp = strcmp(A->info.lex, cl);

    if (comp == 0) { //cl == A->info
        return 1;
    }
    else if (comp > 0) { //cl > A->info
        return treeIsMemberByLex(treeL(A), cl);
    }
    //cl < A->info
    return treeIsMemberByLex(treeR(A), cl);
}

/*
 * Funcion auxiliar recursiva de print de la tabla
 */
void _printTabla(binTree A){

    if (!treeIsEmpty(A)) {
        if (A->treeL != NULL) {
            _printTabla(A->treeL);
        }
        printf("%20s %5d\n", A->info.lex, A->info.id);

        if (A->treeR != NULL) {
            _printTabla(A->treeR);
        }
    }

}


/**
 * Imprime la tabla de simbolos
 */
void printTable(binTree A) {
    printf("+++++++++++======SIMBOLOS======+++++++++++\n");
    _printTabla(A);
}