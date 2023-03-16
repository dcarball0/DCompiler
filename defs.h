#ifndef DEFSH
#define DEFSH
/*
* Tabla de simbolos
*	111x Palabras reservadas
*	222x Tipos de datos
*	333x Variable
* 
*	999x Operadores doble simbolo
*	
*/

// Palabras reservadas
#define DIDENTIFIER 1110
#define DIMPORT 1111
#define DWHILE 1112
#define DFOR 1113
#define DIF 1114
#define DELSE 1115
#define DFOREACH 1116
#define DRETURN 1117
#define DCAST 1118

// Tipos de datos
#define DDOUBLE 2221
#define DINT 2222
#define DFLOAT 2223
#define DVOID 2224

// Variables
#define DINTEGER 3332
#define DFLOATING 3333
#define DSTRING 3334

// Operandos
#define PLUSPLUS 9990
#define PLUSEQUALS 9991
#define EQUALSEQUALS 9992

#endif