#include "binTree.h"
#include "input.h"
#include "defs.h"
#include <ctype.h>
#include "symbols.h"
#include "errorHandler.h"

/*
*  RECONOCIMIENTO DE FLOATS
*/
// Flotantes 1.XXXX
void doFloating(char nC) {

	if (nC == '.') {
		// Avanzar hasta que se acabe el float
		// o encontrar exponente
		// 1.0_00 ---- 1.0_00e...
		nC = nextChar();
		while (isdigit(nC) || nC == '_')
		{
			nC = nextChar();
		}

		// Encontramos exponente
		// Buscar patron 1.123e+12_3 1.123e-1 1.123e2
		if (nC == 'e' || nC == 'E') {
			// 1.0_00e
			nC = nextChar();
			if (nC == '+' || nC == '-' || isdigit(nC) || nC == '_')
			{
				// 1.0_00+
				nC = nextChar();
				while (isdigit(nC) || nC == '_')
				{
					nC = nextChar();
				}
				// 1.0_00+12_3
			}
			else
			{
				dCompError(ERR_BAD_NUMBER);
			}
		}
	}
	else if (nC == 'e' || nC == 'E') {
		// Encontramos exponente
		// Buscar patron 1e+12_3 1e-1 1e2
		// 1.0_00e
		nC = nextChar();
		if (nC == '+' || nC == '-' || isdigit(nC) || nC == '_')
		{
			// 1e+ 1e0
			nC = nextChar();
			while (isdigit(nC) || nC == '_')
			{
				nC = nextChar();
			}
			// 1e+12_3
		}
		else
		{
			dCompError(ERR_BAD_NUMBER);
		}
	}
}

void doInteger(char nC) {
	// Leer entero 012312_3123 hasta que no sea digito
	while (isdigit(nC) || nC == '_')
	{
		nC = nextChar();
	}
	// Retroceder un caracter para poder leerlo bien 
	returnPointer();
}

void doBinary(char nC) {
	// Leer entero 012312_3123 hasta que no sea digito
	while (nC == '0' || nC == '1' || nC == '_')
	{
		nC = nextChar();
	}
	// Retroceder un caracter para poder leerlo bien 
	returnPointer();
}

/*
* Automata que devuelve el siguiente componente lexico
*	@param comp: componente donde se devuelve el siguiente componente lexico
*/
int nextLexComp(lexComp* comp) {
	int accept;
	int state;
	char nC;

	accept = 0;
	state = 0;
	nC = '\0';

	while (!accept) {
		switch (state) 
		{
			/*
			* CASO Inicial - Comprobar en que tipo de lexema estamos
			*/
			case 0:
				// Obtener siguiente caracter
				nC = nextChar();

				// Estos no se imprimen
				if (nC == ' ' || nC == '\t' || nC == '\r' || nC == '\n')
				{
					// Pasar a siguiente componente
					restartPointers();
				}
				// Si es un caracter separador no entra en otro estado
				else if (nC == '.' || nC == ',' || nC == ';' ||
					nC == '?' || nC == ':' || nC == '{' || 
					nC == '}' || nC == '(' ||  nC == ')' || 
					nC == '[' || nC == ']')
				{
					// IDs de estos elementos son su codigo ascii
					comp->id = nC;
					// Copiar lexema a componente
					getLex(comp);
					// Aceptar cadena
					accept = 1;
				}
				// Parar en fin de texto
				else if (nC == -2)
				{
					return 0;
				}
				// Si es un identificador
				else if (isalpha(nC) || nC == '_')
				{
					state = 1;
				}
				// Si es un comentario
				else if (nC == '/')
				{
					state = 2;
				}
				// Si es un operador doble
				else if (nC == '+' || nC == '=' || nC == '-' || nC == '*' || 
					nC == '%' || nC ==  '&' || nC == '^' || nC == '|' || 
					nC == '!' || nC == '>' || nC == '<' || nC == '~')
				{
					state = 3;
				}
				// Si es una string
				else if (nC == '"')
				{
					state = 4;
				}
				// Si es un numero
				else if (isdigit(nC))
				{
					state = 5;
				}
				break;

			/*
			* CASO Identificadores - std, nombres variables, etc
			*/
			case 1:
				// Obtener siguiente caracter
				nC = nextChar();

				// Mientras sigamos en el mismo lexema, seguir avanzando
				while (isalpha(nC) || isdigit(nC) || nC == '_')
				{
					nC = nextChar();
				}

				// Retroceder puntero una vez llega a caracter separador
				if (nC != EOF)
				{
					returnPointer();
				}

				// Caso default si no es palabra reservada
				comp->id = DIDENTIFIER;
				// Devuelve lexema entre punteros
				getLex(comp);
				// Si es palabra reservada aqui devuelve el codigo especifico
				getID(comp);
				// Aceptar cadena para guardar componente lexico
				accept = 1;
				break;

			/*
			* CASO Comentarios - Anidados y no anidados
			*/
			case 2:
				// Obtener siguiente caracter
				nC = nextChar();

				// Si es un comentario de "//" se acaba en el salto de linea
				if (nC == '/')
				{
					while (nC != '\n')
					{
						nC = nextChar();
					}

					// Reiniciar estado ya que no lo imprimimos
					state = 0;
					// Pasar a siguiente componente
					restartPointers();
					break;
				}

				// Si es un comentario de /* buscar final */
				else if (nC == '*')
				{
					// Aqui el comentario ya estaría abierto
					// Comprobar cuando cierra (* y / juntos)
					nC = nextChar();

					// Variable para comprobar un conjunto de * y /
					bool closeComment;
					closeComment = false;

					// Iterar por el comentario buscando su cierre
					while (!closeComment)
					{
						if (nC == '*')
						{
							// Una vez encontrado asterisco, buscar si es cierre de comentario
							nC = nextChar();
							if (nC == '/')
							{
								closeComment = true;
							}
						}
						// Si no es asterisco seguir saltandose el comentario
						nC = nextChar();
					}

					// Reiniciar estado ya que no lo imprimimos
					state = 0;
					// Pasar a siguiente componente
					restartPointers();
					break;
				}

				// Si es un comentario anidado /+ /++/ +/
				else if (nC == '+')
				{
					// Variable para contar numero de comentarios abiertos
					short nestedCount;
					nestedCount = 1;

					// Iterar por el comentario buscando su cierre o posibles comentarios anidados
					while (nestedCount != 0)
					{
						nC = nextChar();
						// Si encontramos un +, comprobar si es cierre de comentario
						if (nC == '+')
						{
							// Una vez encontrado asterisco, buscar si es cierre de comentario
							nC = nextChar();
							if (nC == '/')
							{
								// Restar un comentario abierto
								nestedCount--;
							}
						}
						// Si encontramos un /, comprobar si es apertura (else if para evitar que entre en un /+ +/+)
						else if (nC == '/')
						{
							// Una vez encontrado /, buscar si es apertura de comentario
							nC = nextChar();
							if (nC == '+')
							{
								// Sumar uno a la cuenta de comentarios abiertos
								nestedCount++;
							}
						}
					}

					// Reiniciar estado ya que no lo imprimimos
					state = 0;
					// Pasar a siguiente componente
					restartPointers();
					break;
				}
				// Si es un /= 
				else if (nC == '=')
				{
					// Tomamos su id de la tabla
					comp->id = DIVEQUALS;
					// Devuelve lexema entre punteros
					getLex(comp);
					// Aceptar cadena para guardar componente lexico
					accept = 1;
				}
				// Si es un / de division
				else
				{
					// Retroceder un caracter para poder leerlo bien
					returnPointer();
					// Tomamos su id como su codigo ascii
					comp->id = nC;
					// Devuelve lexema entre punteros
					getLex(comp);
					// Aceptar cadena para guardar componente lexico
					accept = 1;
				}
			break;
			/*
			* CASO Operadores dobles - Comprobar cual es, en el codigo solo hay ++, += y ==
			*/
			case 3:
				// Comprobar en que comienzo de doble operador estamos
				if (nC == '+') 
				{
					// Comprobar final de doble operador += o ++
					nC = nextChar();
					if (nC == '+')
					{
						// Tomamos su id de la tabla
						comp->id = PLUSPLUS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					else if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = PLUSEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un +)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '=') 
				{
					// Comprobar final de doble operador ==
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = EQUALSEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '-')
				{
					// Comprobar final de doble operador -= o --
					nC = nextChar();
					if (nC == '-')
					{
						// Tomamos su id de la tabla
						comp->id = MINUSMINUS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					else if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = MINUSEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un -)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '*')
				{
					// Comprobar *= o *
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = MULTEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un *)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '%') 
				{
					// Comprobar %= o %
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = MODEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un %)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '~')
				{
					// Comprobar ~= o ~
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = MULTEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un ~)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '&')  
				{
					// Comprobar &= o && o &
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = BITEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					else if (nC == '&')
					{
						// Tomamos su id de la tabla
						comp->id = LOGAND;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un &)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '^')  
				{
					// Comprobar ^= o ^^= o ^
					nC = nextChar();
					if (nC == '^') // ^^
					{
						// Comprobar ^^= o ^^
						nC = nextChar();
						if (nC == '=') // ^^=
						{
							// Tomamos su id de la tabla
							comp->id = BITEXEQUALS;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;

						}
						else // ^^
						{
							// Retroceder puntero una vez llega a caracter separador
							returnPointer();
							// Tomamos su id de la tabla
							comp->id = BITXOR;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;
						}
					}
					// No es doble operador (Solo un ^)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '|')
				{
					// Comprobar |= o || o |
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = BITINEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					else if (nC == '|')
					{
						// Tomamos su id de la tabla
						comp->id = LOGOR;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un |)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '!')
				{
					// Comprobar != o !
					nC = nextChar();
					if (nC == '=')
					{
						// Tomamos su id de la tabla
						comp->id = NOTEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
					// No es doble operador (Solo un !)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '<')
				{
					// Comprobar <= o <<= o <
					nC = nextChar();
					if (nC == '<') // <<
					{
						// Comprobar <<= o <<
						nC = nextChar();
						if (nC == '=') // <<=
						{
							// Tomamos su id de la tabla
							comp->id = LSHIFTEQUALS;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;
							
						}
						else // <<
						{
							// Retroceder puntero una vez llega a caracter separador
							returnPointer();
							// Tomamos su id de la tabla
							comp->id = BINLSHIFT;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;
						}
					}
					else if (nC == '=') // <=
					{
						// Tomamos su id de la tabla
						comp->id = LESSEQUALS;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;

					}
					// No es doble operador (Solo un <)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				else if (nC == '>')
				{
					// Comprobar >= o >>= o >
					nC = nextChar();
					if (nC == '>') // >>
					{
						// Comprobar >>= o >>
						nC = nextChar();
						if (nC == '=') // >>=
						{
							// Tomamos su id de la tabla
							comp->id = RSHIFTEQUALS;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;

						}
						else if (nC == '=') // >=
						{
							// Tomamos su id de la tabla
							comp->id = MOREEQUALS;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;

						}
						else // >>
						{
							// Retroceder puntero una vez llega a caracter separador
							returnPointer();
							// Tomamos su id de la tabla
							comp->id = BINRSHIFT;
							// Devuelve lexema entre punteros
							getLex(comp);
							// Aceptar cadena para guardar componente lexico
							accept = 1;
						}
					}
					// No es doble operador (Solo un >)
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();
						// Devuelve lexema entre punteros
						comp->id = nC;
						// Devuelve lexema entre punteros
						getLex(comp);
						// Aceptar cadena para guardar componente lexico
						accept = 1;
					}
				}
				break;
			/*
			* CASO Strings - Hacer igual que comentarios //
			*/
			case 4:
				// Recorrer string hasta que cierre
				while (1) 
				{
					// Avanzar en string
					nC = nextChar();
					// Se pueden escapar haciendo \", comprobamos si estan juntos
					if (nC == '\\')
					{
						nC = nextChar();
						if (nC == '"')
						{
							nC = nextChar();
						}
					}
					// Si es fin de string
					else if (nC == '"') 
					{
						comp->id = DSTRING;
						// Salimos del bucle
						break;
					}
					// No se cierra string salir del bucle
					else if (nC == EOF) 
					{
						dCompError(ERR_UNCLOSED_STRING);
						// Salir del bucle
						break;
					}
				}
				
				// Tomamos su lexema a partir del id
				getLex(comp);
				// Aceptar cadena para guardar componente lexico
				accept = 1;
				break;
			/*
			* CASO Numeros - Comprobar tipo
			*/
			case 5:
				// Preasignamos a int
				comp->id = DINTEGER;
				// Si empieza por 0 puede ser binario
				if (nC == '0')
				{
					// Si el siguiente caracter es b o B es binario 100%
					nC = nextChar();
					if (nC == 'b' || nC == 'B')
					{
						nC = nextChar();

						doBinary(nC);

						getLex(comp);
					}
					// 0. es float
					else if (nC == '.') {
						doFloating(nC);

						getLex(comp);
					}
					// 0 solo o 0123 o 0123.
					else
					{
						doInteger(nC);

						getLex(comp);
					}
				}
				// Si no es binario y no es float de 0.
				// Puede ser entero o float de x.
				else
				{
					// Avanzar hasta que se acabe el numero o su parte entera
					nC = nextChar();

					bool endInt;
					endInt = false;

					while (!endInt)
					{
						// Parte entera
						while (isdigit(nC) || nC == '_') {
							nC = nextChar();
						}
						// Es float
						if (nC == '.' || nC == 'e' || nC == 'E')
						{
							// Establecer id como tipo flotante
							comp->id = DFLOATING;
							// Acabamos parte entera
							endInt = true;

							doFloating(nC);
						}
						// Es entero
						else
						{
							endInt = true;
						}
					}

					// Retroceder un caracter para poder leerlo bien 
					returnPointer();
					getLex(comp);
				}
				
				accept = 1;
				break;
		}
	}
	return comp->id;
}