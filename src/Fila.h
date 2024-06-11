#ifndef _Fila_H_
#define _Fila_H_

#include<stdio.h>
#include<stdlib.h>
#include"String.h"

typedef struct Fila
{
	void* primero;
} Fila;

Fila* crear_fila()
{
	Fila* fila = (Fila*) calloc(1, sizeof(Fila));
	return fila;
}

void agregar_elemento(Fila* fila, Elemento* ne)
{
	Elemento* e = fila->primero;
	if(e)
	{
		while(e->siguiente)
		{
			e = e->siguiente;
		}
		e->siguiente = ne;
	}
	else
	{
		fila->primero = ne;
	}
}

void imprimir_elementos(Fila* fila)
{
	Elemento* e = fila->primero;
	int i = 0;
	while(e)
	{
		i++;
		printf("#%d %s\n", i, e->dato->s);
		e = e->siguiente;
	}
}

Elemento* get_elemento(Fila* fila)
{
	Elemento* e = fila->primero;
	fila->primero = e->siguiente;
	e->siguiente = NULL;
	return e;
}

Elemento* remove_last(Fila* fila)
{
	Elemento* ret = NULL;
	if(fila)
	{
		Elemento* e = fila->primero;
		Elemento* prev;
		while(e && e->siguiente)
		{
			prev = e;
			e = e->siguiente;
		}
		prev->siguiente = NULL;
		ret = e;
	}
	return ret;
}

#endif
