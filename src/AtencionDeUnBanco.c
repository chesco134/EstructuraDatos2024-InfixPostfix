#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"Fila.h"
#include"String.h"

typedef struct Cliente
{
	String* nombre; // Aproximadamente 10 chars
	String* numero_de_cuenta; // 8 chars
	int tipo_de_transaccion; // 1 - Consultar, 2 - Retirar, 3 - Depositar
	struct Cliente* siguiente;
} Cliente;

// Leer archivo
// Crear una fila
// Aleatoriamente
// Separar la fila en 2: clientes y clientes VIP
// Cada fila se guardará en un archivo.

void put_contents_to_queue(FILE* f, Fila* fila);
Cliente* crear_cliente(String* nombre, String* numero_de_cuenta, int tipo_de_transaccion);
void agregar_cliente(Fila* fila, Cliente* cliente);
void imprime_clientes(Fila* fila, char* fname);
Cliente* get_cliente(Fila* fila);

int main(int argc, char** argv)
{
	if(argc > 2)
	{
		FILE* f = fopen(argv[1], "r");
		Fila* f_arribo = crear_fila();
		Fila* f_normales = crear_fila();
		Fila* f_vip = crear_fila();
		int seed = atoi(argv[2]);
		if(f)
		{
			put_contents_to_queue(f, f_arribo);
			fclose(f);
			srand(seed);
			Cliente* cliente = get_cliente(f_arribo);
			while(cliente)
			{
				if( (rand() % 10) <=4 )
				{
					agregar_cliente(f_normales, cliente);
				}
				else
				{
					agregar_cliente(f_vip, cliente);
				}
				cliente = get_cliente(f_arribo);
			}
			printf("\n*** Clientes VIP ***\n\n");
			imprime_clientes(f_vip, "Clientes_VIP");
			printf("\n*** Clientes Normales ***\n\n");
			imprime_clientes(f_normales, "Clientes_Normales");
		}	
	}
	else
	{
		printf("Debe ingresar una ruta a un archivo con el formato correcto de clientes de un banco, y un número entero como semilla para inicializar los valores de RNG.\n");
	}
	return 0;
}

Cliente* get_cliente(Fila* f)
{
	if(f)
	{
		Cliente* c = f->primero;
		if(c)
		{
			f->primero = c->siguiente;
			c->siguiente = NULL;
			return c;
		}
		return NULL;
	}
	return NULL;
}

void imprime_clientes(Fila* fila, char* file_name)
{
	if(fila)
	{
		FILE* f = fopen(file_name, "w");
		if(f)
		{
			Cliente* cliente = fila->primero;
			while(cliente)
			{
				fprintf(f, "Cliente: %s, con número de cuenta: %s, hará la transacción número: %d.\n", cliente->nombre->s, cliente->numero_de_cuenta->s, cliente->tipo_de_transaccion);
				cliente = cliente->siguiente;
			}
			fclose(f);
		}
	}
}

void put_contents_to_queue(FILE* f, Fila* fila)
{
	char* c = (char*) calloc(1, sizeof(char));
	char* s = NULL;
	int chars_read;
	int count = 0;
	int i;
	int j;
	int k;
	char* elemento = (char*) calloc(1, sizeof(char));
	String* entry = NULL;
	String** row = (String**) calloc(3, sizeof(String*));
	do
	{
		chars_read = fread(c, sizeof(char), 1, f);
		if(chars_read != 0)
		{
			if('\n' == c[0])
			{
				k = 0;
				j = 0;
				for(i = 0; i < count; i++)
				{
					if('\t' != s[i])
					{
						k++;
						elemento = add_char_to_pointer(elemento, k, s[i]);
					}
					else
					{
						row[j] = string(elemento, k);
						j++;
						elemento = NULL;
						k = 0;
					}
				}
				row[j] = string(elemento, k);
				agregar_cliente(fila, crear_cliente(row[0], row[1], atoi(row[2]->s)));
				s = NULL;
				count = 0;
			}
			else
			{
				count += chars_read;
				s = add_char_to_pointer(s, count, c[0]);
			}
		}
	}while(chars_read != 0);
	if(count > 0)
	{
		k = 0;
		j = 0;
		for(i = 0; i<count; i++)
		{
			if('\t' != s[i])
			{
				k++;
				elemento = add_char_to_pointer(elemento, k, s[i]);
			}
			else
			{
				row[j] = string(elemento, k);
				j++;
				elemento = NULL;
				k = 0;
			}
		}
		row[j] = string(elemento, k);
		agregar_cliente(fila, crear_cliente(row[0], row[1], atoi(row[2]->s)));
	}
}

Cliente* crear_cliente(String* nombre, String* numero_de_cuenta, int tipo_de_transaccion)
{
	Cliente* cliente = (Cliente*) calloc(1, sizeof(Cliente));
	cliente->nombre = nombre;
	cliente->numero_de_cuenta = numero_de_cuenta;
	cliente->tipo_de_transaccion = tipo_de_transaccion;
	return cliente;
}

void agregar_cliente(Fila* fila, Cliente* cliente_nuevo)
{
	if(fila)
	{
		Cliente* cliente = fila->primero;
		if(cliente)
		{
			while(cliente->siguiente)
			{
				cliente = cliente->siguiente;
			}
			cliente->siguiente = cliente_nuevo;
		}
		else
		{
			fila->primero = cliente_nuevo;
		}

	}
}
