#include<stdlib.h>
#include<string.h>
#include"Fila.h"
#include"String.h"

int main(int argc, char** argv)
{
	if(argc > 1)
	{
		Fila* fila = crear_fila();
		Elemento* e;
		int i;
		for(i = 1; i<argc; i++ )
		{
			e = crear_elemento(string(argv[i], strlen(argv[i])));
			agregar_elemento(fila, e);
		}
		printf("Los elementos son:\n");
		imprimir_elementos(fila);
	}
	else
	{
		char* s;
		int read;
		Fila* fila = crear_fila();
		Elemento* e;
		do
		{
			s = (char*) calloc(100, sizeof(char));
			printf("Escriba una palabra: ");
			read = scanf("%s", s);
			e = crear_elemento(string(s, read));
			agregar_elemento(fila, e);
		}while(strcmp(s, "NO") != 0);
		remove_last(fila);
		printf("Lo que se ingresó es: \n");
		imprimir_elementos(fila);
	}
	return 0;
}
