#ifndef _String_H_
#define _String_H_

#include<stdlib.h>

typedef struct String
{
	char* s;
	int length;
} String;

typedef struct Elemento
{
	String* dato;
	struct Elemento* siguiente;
} Elemento;

String* string(char* s, int length)
{
	String* ns = (String*) calloc(1, sizeof(String));
	ns->s = s;
	ns->length = length;
	return ns;
}

char* add_char_to_pointer(char* s, int count, char c)
{
	if(s)
	{
		s = (char*) reallocarray(s, count, sizeof(char));
		s[count-1] = c;
	}
	else
	{
		s = (char*) calloc(1, sizeof(char));
		s[0] = c;
	}
	return s;
}

Elemento* crear_elemento(String* dato)
{
	Elemento* e = (Elemento*) calloc(1, sizeof(Elemento));
	e->dato = dato;
	e->siguiente = NULL;
	return e;
}


#endif
