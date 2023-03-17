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
#define DRESERVED 1110 // Para los tipos que no estan en regresion y no matarme poniendo 47 tipos
#define DIMPORT 1111
#define DWHILE 1112
#define DFOR 1113
#define DIF 1114
#define DELSE 1115
#define DFOREACH 1116
#define DRETURN 1117
#define DCAST 1118
#define DIDENTIFIER 11111

// Tipos de datos
#define DDATATYPE 2220 // Para los tipos que no estan en regresion y no matarme poniendo 47 tipos
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
#define MINUSMINUS 9992
#define MINUSEQUALS 9993
#define MULTEQUALS 9994
#define MODEQUALS 9995
#define BITEQUALS 9996
#define BITEXEQUALS 9997
#define BITINEQUALS 9998
#define DIVEQUALS 9999
#define NOTEQUALS 99910
#define LESSEQUALS 99911
#define MOREEQUALS 99912
#define LSHIFTEQUALS 99913
#define RSHIFTEQUALS 99914
#define COMPEQUALS 99915
#define EQUALSEQUALS 99920
#define LOGAND 99930
#define LOGOR 99931
#define BITXOR 99932
#define BINLSHIFT 99933
#define BINRSHIFT 99934

#endif