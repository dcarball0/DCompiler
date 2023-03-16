#include "binTree.h"
#include "input.h"
#include "defs.h"
#include <ctype.h>

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

				// Si es un caracter separador
				if (nC == ' ' || nC == '\t' || nC == '\r' || nC == '\n' || nC == '.' || nC == ',' || nC == ';' ||
					nC == '-' || nC == '*' || nC == '%' || nC == '<' || nC == '>' || nC == '!' || nC == '?' ||
					nC == '|' || nC == '^' || nC == '~' || nC == ':' ||
					nC == '{' || nC == '}' || nC == '(' || nC == ')' || nC == '[' || nC == ']')
				{
					comp->id = nC;
					getLex(comp);
					accept = 1;
				}
				// Si es un identificador
				else if (isalpha(nC) || nC == '_')
				{
					state = 1;
				}
				// Si es un numero
				else if (isdigit(nC))
				{
					state = 2;
				}
				// Si es un comentario
				else if (nC == '/')
				{
					state = 3;
				}
				// Si es un operador doble
				else if (nC == '+' || nC == '=')
				{
					state = 4;
				}
				// Si es una string
				else if (nC == '"')
				{
					state = 5;
				}
				else if (nC == EOF)
				{
					return 0;
				}
				break;

			/*
			* CASO Identificadores - std, nombres variables, etc
			*/
			case 1:
				// Obtener siguiente caracter
				nC = nextChar();

				// Mientras sigamos en el mismo lexema
				while (isalpha(nC) || isdigit(nC) || nC == '_')
				{
					nC = nextChar();
				}

				// Retroceder puntero una vez llega a caracter separador
				if (nC != EOF)
					returnPointer();

				// Devuelve lexema entre punteros
				getLex(comp);

				// Devuelve el identificador relacionado al lexema
				//getLexID();
				comp->id = 300;
				accept = 1;
				break;

			/*
			* CASO Numeros - Comprobar tipo
			*/
			case 2:
				// Obtener siguiente caracter
				nC = nextChar();
				comp->id = INT;
				accept = 1;
				break;
			/*
			* CASO Comentarios - Anidados y no anidados
			*/
			case 3:
				// Obtener siguiente caracter
				nC = nextChar();

				// Si es un comentario de "//" se acaba en el salto de linea
				if (nC == '/')
				{
					while (nC != '\n')
					{
						nC = nextChar();
					}

					state = 0;
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

					state = 0;
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
								nestedCount--;
							}
						}
						// Si encontramos un /, comprobar si es apertura (else if para evitar que entre en un /+ +/+)
						else if (nC == '/')
						{
							// Una vez encontrado +, buscar si es cierre de comentario
							nC = nextChar();
							if (nC == '+')
							{
								nestedCount++;
							}
						}
					}

					state = 0;
					restartPointers();
					break;
				}
				else
				{
					comp->id = nC;
					getLex(comp); // todo
					accept = 1;
				}
			break;
			/*
			* CASO Operadores dobles - Comprobar cual es, en el codigo solo hay ++, += y ==
			*/
			case 4:
				// Comprobar en que comienzo de doble operador estamos
				if (nC == '+') 
				{
					// Comprobar final de doble operador += o ++
					nC = nextChar();
					if (nC == '+')
					{
						// Devuelve lexema entre punteros
						getLex(comp);

						comp->id = PLUSPLUS;
						accept = 1;
					}
					else if (nC == '=')
					{
						// Devuelve lexema entre punteros
						getLex(comp);

						comp->id = PLUSEQUALS;
						accept = 1;
					}
					// No es doble operador
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();

						comp->id = nC;
						getLex(comp);
						accept = 1;
					}
				}
				else if (nC == '=') 
				{
					// Comprobar final de doble operador ==
					nC = nextChar();
					if (nC == '=')
					{
						// Devuelve lexema entre punteros
						getLex(comp);

						comp->id = EQUALSEQUALS;
						accept = 1;
					}
					// No es doble operador
					else {
						// Retroceder puntero una vez llega a caracter separador
						returnPointer();

						comp->id = nC;
						getLex(comp);
						accept = 1;
					}
				}
				break;
			/*
			* CASO Strings - Hacer igual que comentarios //
			*/
			case 5:
				bool closeString;
				closeString = 0;

				while (!closeString) 
				{
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
					else if (nC == '"') {

						closeString = 1;
					}
				}

				state = 0;
				comp->id = DSTRING;
				getLex(comp);
				accept = 1;
				break;
		}
	}
	return comp->id;
}