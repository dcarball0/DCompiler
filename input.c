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
	bool dontLoad;
	int line;
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
	c.line = 0;

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
		// Inicializar centinela
		initInput();

		// Cargar contenido de primer bloque
		loadBlock();
	}
	else
	{
		dCompError(ERR_FILE_OPEN);
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

	// Contar diferencia de caracteres para ver cuantos lee
	// Si lee menos que N es que se acabo el archivo
	short oldInPos;
	oldInPos = inPos;
	
	// Sumar el numero de caracteres leidos a la posicion
	inPos += (int)fread(c.block[c.curBlock], sizeof(char), N, in);

	// Para cuando
	c.block[c.curBlock][inPos-oldInPos] = EOF;
}

/*
*	Actualizar puntero Centinela
*	@return char: caracter actual en centinela
*/
char nextChar() {
	// Leer caracter
	char r;
	r = c.block[c.curBlock][c.end - (N * c.curBlock)]; // !c.curBlock porque ya cambiamos la variable en loadBlock()

	// Contar lineas para errores
	if (r == '\n')
	{
		c.line++;
	}
	
	// Comprobar EOF - Llegar al final del bloque o final de archivo
	if (r == EOF)
	{
		// Final de bloque
		if (!feof(in))
		{
			// Cambiar bloque actual
			c.curBlock = !c.curBlock;

			// Puntero al principio del bloque actual
			c.end = c.curBlock*N;

			// Cargar bloque
			if (!c.dontLoad)
			{
				loadBlock();
			}
			// Si ya pasamos la iteracion sin cargar, reiniciar parametro
			else 
			{
				c.dontLoad = false;
			}

			// Pasar a siguiente caracter si no acabo lexema en bloque
			if (c.start != N)
			{
				r = nextChar();
			}
			// No volver a cargar el bloque B al volver a pasar
			else {
				c.end++;
				r = c.block[c.curBlock][c.start - (N * c.curBlock)];
			}
			
			return r;
		}
		// Fin de archivo
		else 
		{
			// Codigo para acabar archivo
			return -2;
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
		c.end = (N * 2);
	}

	// Lexema acaba justo al final del bloque A
	else if (c.curBlock == B && c.end == (N + 1)) 
	{
		// Cambiamos al bloque anterior
		c.curBlock = A;

		// No volver a cargar el bloque B al volver a pasar
		c.dontLoad = true;

		// Cambiamos el puntero al final del bloque A
		c.end = N;
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
	lexSize = 1;

	// Obtener lexema a partir de id (reemplazar cadena por STRING)
	switch (lex->id)
	{
		case DSTRING:
			// Reservar memoria para el lexema
			lex->lex = (char*)malloc(sizeof(char) * (6 + 1));
			if (lex->lex)
			{
				strcpy(lex->lex, "STRING");
			}
			
			restartPointers();
			return;
		default:
			break;
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
		dCompError(ERR_OVERFLOW_INTERNAL_BUFFER);

		// Reiniciar punteros
		restartPointers();
	}
	// CASO Lexema es menor que el tamaño de bloque
	else
	{
		// Reservar memoria para el lexema
		lex->lex = (char*)malloc(sizeof(char) * (lexSize + 1));
		if (!lex->lex) return;

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
			//  ->ema---|
			strncpy(lex->lex + (N * 2) - c.start, c.block[A], c.end);
		}
		// Caso normal
		// --lexema--|------
		else
		{
			// Si bloque A curBlock=0 -- bloqueA+start
			// Si bloque B curBlock=1 -- bloqueB-N+start
			// no preguntes
			strncpy(lex->lex, c.block[c.curBlock] - (N*c.curBlock) + c.start, c.end - c.start);
		}

		// Terminar string
		lex->lex[lexSize] = '\0';
		// Reiniciar punteros
		restartPointers();
	}
}