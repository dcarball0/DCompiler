#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "errores.h"

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
	Estructura doble centinela
	block:			Bloque A y B
	start:		Puntero inicial
	end:		Puntero final
	curBlock:	Siguiente bloque a cargar (Se actualiza en loadBlock())
*/ 
typedef struct {
	char block[2][N + 1];
	int start;
	int end;
	bool curBlock;
} CENTINELA;

/*
	Variables globales

	in: Archivo entrada
	inPos: Posicion actual en archivo
	c: Centinela
*/
FILE* in;
int inPos;
CENTINELA c;

/*
*	Inicialización Sistema de entrada
*/
void init() {
	// Inicializar estructura centinela
	c.start = 0;
	c.end = 0;
	c.curBlock = A;

	// Inicializar EOF en centinela
	c.block[A][N] = EOF;
	c.block[B][N] = EOF;

	// Inicializar posicion en el archivo
	inPos = 0;
}

/*
*	Abrir archivo d
*	@param dir: Nombre fichero de entrada
*/
void dOpen(char* dir) {
	// Abrir archivo
	if ((in = fopen(dir, "r")) != NULL) 
	{
		initCentinela();

		// Cargar contenido de primer bloque
		loadBlock();
	}
	else
	{
		// TODO: Gestion de errores
		perror("Error al abrir el archivo.");
		exit(0);
	}
}

/*
*	Cerrar archivo
*/
void dClose() {
	fclose(in);
}

/*
*	Funcion para cargar siguiente bloque al Centinela
*/
void loadBlock() {
	// Actualizar puntero a posicion actual
	fseek(in, inPos, SEEK_SET);
	
	// Sumar el numero de caracteres leidos a la posicion
	inPos += fread(c.block[c.curBlock], sizeof(char), N, in);

	//TODO: Comprobar si lexema sobrepasa al final de bloque
}

/*
*	Actualizar puntero Centinela
*	@return char: caracter actual en centinela
*/
char nextChar() {
	// Leer caracter
	char r;
	r = c.block[!c.curBlock][c.end]; // !c.curBlock porque ya cambiamos la variable en loadBlock()

	// Comprobar EOF - Llegar al final del bloque o final de archivo
	if (r == EOF)
	{
		// Final de bloque
		if (!feof(in))
		{
			// Cambiar bloque actual
			c.curBlock = !c.curBlock;

			//TODO: Comprobar retroceder

			r = nextChar();
		}
		// Final de archivo
		else
		{
			c.end++;
		}
	}
	// Si no se leyo EOF, continuar puntero final centinela
	else
	{
		c.end++;
	}

	
}