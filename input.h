#ifndef INPUT_H
#define INPUT_H

#include "binTree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "errorHandler.h"

/*
* Definiciones
* N: Potencia de la unidad de asignacion
* A: Referencia a bloque en centinela
* B: Referencia a bloque en centinela
*/
#define N 64
#define A 0
#define B 1

/*
*	Inicialización Sistema de entrada
*/
void initInput();

/*
*	Abrir archivo d
*	@param dir: Nombre fichero de entrada
*/
void dOpen(char* dir);

/*
*	Cerrar archivo
*/
void dClose();

/*
*	Funcion para cargar siguiente bloque al Centinela
*/
void loadBlock();

/*
*	Actualizar puntero Centinela
*	@return char: caracter actual en centinela
*/
char nextChar();

/*
*	Hace retroceder al puntero delantero para que se posicione en el final del lexema
*/
void returnPointer();

/*
*	Reiniciar posicion puntero inicial a puntero final
*/
void restartPointers();

/*
*	Devuelve el lexema actual en <lex>
*	@param lex: variable en la que se recogerá el lexema
*/
void getLex(lexComp *lex);

#endif