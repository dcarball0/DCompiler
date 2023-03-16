#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> 
#include "errorHandler.h"
#include "input.h"
#include <string.h>
#include "defs.h"

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
void initInput() {
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
		initInput();

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

	short oldInPos;
	oldInPos = inPos;
	
	// Sumar el numero de caracteres leidos a la posicion
	inPos += (int)fread(c.block[c.curBlock], sizeof(char), N, in);
}

/*
*	Actualizar puntero Centinela
*	@return char: caracter actual en centinela
*/
char nextChar() {
	// Leer caracter
	char r;
	r = c.block[c.curBlock][c.end - (N * c.curBlock)]; // !c.curBlock porque ya cambiamos la variable en loadBlock()

	// Comprobar EOF - Llegar al final del bloque o final de archivo
	if (r == EOF)
	{
		// Final de bloque
		if (!feof(in))
		{
			// Cambiar bloque actual
			c.curBlock = !c.curBlock;

			c.end = c.curBlock*(N + 1);

			loadBlock();

			//TODO: Comprobar retroceder

			r = nextChar();
		}
	}
	// Si no se leyo EOF, continuar puntero final centinela
	else
	{
		c.end++;
	}	
	return r;
}

/*
*	Hace retroceder al puntero delantero para que se posicione en el final del lexema
*/
void returnPointer() {
	// Lexema acaba justo al final del bloque B
	if (c.curBlock == A && c.end == 0) 
	{
		// Cambiamos al bloque anterior
		c.curBlock = B;

		// Cambiamos el puntero al final del bloque B
		c.end = (N * 2) - 1;
	}

	// Lexema acaba justo al final del bloque A
	else if (c.curBlock == A && c.end == (N + 1)) 
	{
		// Cambiamos al bloque anterior
		c.curBlock = A;

		// Cambiamos el puntero al final del bloque A
		c.end = N - 1;
	}
	// Caso normal, lexema acaba en mitad de un bloque
	else 
	{
		c.end--;
	}
}

/*
*	Reiniciar posicion puntero inicial a puntero final
*/
void restartPointers() {
	c.start = c.end;
}

/*
*	Devuelve el lexema actual en <lex>
*	@param lex: variable en la que se recogerá el lexema
*/
void getLex(lexComp *lex) {
	// Tamaño lexema
	int lexSize;
	lexSize = 0;

	switch (lex->id)
	{
		case DSTRING:
			// Reservar memoria para el lexema
			lex->lex = (char*)malloc(sizeof(char) * (6 + 1));
			strcpy(lex->lex, "STRING");
			restartPointers();
			return;
	}

	// Si el lexema se encuentra entre el final del bloque B y el principio del bloque A
	// ema---|---lex
	if (c.start > c.end) 
	{
		lexSize = 2 * N - c.start + c.end;
	}
	// Caso normal
	// --lexema--|------
	else
	{
		lexSize = c.end - c.start;
	}

	// CASO Lexema es mas grande que el bloque (condicion para psicopatas)
	// abcdefghijk|lmnopqrst--
	if (lexSize >= N)
	{
		// TODO: Gestion de errores lex size greater than block

		// Reiniciar punteros
		restartPointers();
	}
	// CASO Lexema es menor que el tamaño de bloque
	else
	{
		// Reservar memoria para el lexema
		lex->lex = (char*)malloc(sizeof(char) * (lexSize + 1));

		// Si el lexema se encuentra en la mitad de A y B
		// ---lex|ema---
		if (c.start < N && c.end > N)
		{
			// Copiar primera mitad en bloque A
			// ->lex|
			strncpy(lex->lex, c.block[A] + c.start, N - c.start);

			// Copiar segunda mitad en bloque B
			// |ema<-
			strncpy(lex->lex + N - c.start, c.block[B] , c.end - N);
		}
		// Si el lexema se encuentra en en final de B y el inicio de A
		// ema---|---lex
		else if (c.start > N && c.end < N)
		{
			// Copiar primera mitad en bloque B
			// |--- ->lex
			strncpy(lex->lex, c.block[B] + c.start - N, (N*2) - c.start);

			// Copiar segunda mitad en bloque A
			//  ->ema---
			strncpy(lex->lex + (N * 2) - c.start, c.block[A], c.end);
		}
		// Caso normal
		// --lexema--|------
		else
		{
			// Si bloque A curBlock=0 -- bloqueA+start
			// Si bloque B curBlock=1 -- bloqueB-N+start
			strncpy(lex->lex, c.block[c.curBlock] - (N*c.curBlock) + c.start, c.end - c.start);
		}

		lex->lex[lexSize] = '\0';
		restartPointers();
	}
}