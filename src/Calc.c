#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<math.h>

typedef struct Dato
{
	struct String* s;
	struct Dato* siguiente;
} Dato;

typedef struct Pila
{
	Dato* first;
} Pila;

typedef struct String
{
	char* s;
	int length;
} String;

typedef struct DatoFloat
{
	float d;
	struct DatoFloat* siguiente;
} DatoF;

typedef struct PilaFloat
{
	DatoF* first;
} PilaF;

Pila* create_pila();
Dato* create_dato(String* s);
String* read_file_contents(char* file_path);
Dato* get_dato(Pila* p);
int validate_string(String* s);
String* string(char* s, int length);
int char_in_valid_chars(char, char*);
int validate_infix(String* s);
int validate_decimal_numbers(String* s);
int validate_syntax(String* s);
void highlight_problem(String* s, int position);
void stack_chunks(String* s, Pila* p);
char* add_char_to_pointer(char* p, int count, char c);
void add_dato(Pila* p, Dato* d);
int deep_before_operand(Pila* p);
int is_operator(char c);
int get_weight(char c);
int compare(char c1, char c2);
void add_dato_c(Pila* p, char c);
void dump_stack(Pila* source, Pila* target);
float operate(Pila* p);
DatoF* create_datof(float d);
PilaF* create_pilaf();
void add_datof(PilaF* p, DatoF* d);
DatoF* get_datof(PilaF* p);

int main(int argc, char** argv)
{
	Pila* p = create_pila();
	Pila* p_ = NULL;
	Pila* numeros = NULL;
	float a;
	float b;
	if(argc == 2)
	{
		String* s = read_file_contents(argv[1]);
		if(s)
		{
			int valid = validate_string(s);
			if(valid == 0)
			{
				printf("String after removing spaces: -->%s<-- (of length %d)\n", s->s, s->length);
				valid = validate_infix(s);
				if(valid == 0)
				{
					valid = validate_decimal_numbers(s);
					if(valid == 0)
					{
						valid = validate_syntax(s);
						if(valid == 0)
						{
							printf("POSTFIX string: ");
							stack_chunks(s, p);
							p_ = create_pila();
							dump_stack(p, p_);
							float f = operate(p_);
							printf("\nRESULT: %f\n", f);
							printf("Yes.\n");
						}
						else
						{
							printf("Error at position: %d\n", valid);
							highlight_problem(s, valid);
						}
					}
					else
					{
						if(valid == -1)
						{
							valid = 0;
						}
						printf("Invalid string at position: %d. ", valid);
						highlight_problem(s, valid);
					}
				}
				else
				{
					printf("There was a parentheses missmatch.\n");
					if(valid > 0)
					{
						printf("Missing right parentheses.\n");
					}
					else
					{
						printf("Missing left parentheses.\n");
					}
				}
			}
			else
			{
				printf("Unexpected token at position: ");
				if(valid == -1)
					valid = 0;
				printf("%d. ", valid);
				highlight_problem(s, valid);
			}
			free(s->s);
			free(s);
		}
	}
	printf("\nDone.\n");
	return 0;
}

PilaF* create_pilaf()
{
	PilaF* p = (PilaF*) calloc(1, sizeof(PilaF));
	p->first = NULL;
	return p;
}

DatoF* create_datof(float f)
{
	DatoF* d = (DatoF*) calloc(1, sizeof(DatoF));
	d->siguiente = NULL;
	d->d = f;
	return d;
}

void add_datof(PilaF* p, DatoF* d)
{
	if(p && d)
	{
		if(p->first)
		{
			d->siguiente = p->first;
			p->first = d;
		}
		else
		{
			p->first = d;
		}
	}
}

DatoF* get_datof(PilaF* p)
{
	if(p)
	{
		DatoF* d = p->first;
		if(d)
		{
			p->first = d->siguiente;
			d->siguiente = NULL;
		}
		else
			return NULL;
		return d;
	}
	return NULL;
}

float operate(Pila* p)
{
	float a;
	float b;
	float res;
	char c;
	Dato* d = get_dato(p);
	PilaF* nums = create_pilaf();
	DatoF* num;
	DatoF* primero;
	DatoF* segundo;
	int cnt = 0;
	while(d)
	{
		c = d->s->s[0];
		if(cnt == 0)
			printf("|");
		cnt++;
		printf("%s|", d->s->s);
		if(is_operator(c) == 0 && d->s->length == 1)
		{
			primero = get_datof(nums);
			segundo = get_datof(nums);
			a = segundo->d;
			b = primero->d;
			if( c == '+')
			{
				res = a + b;
			}
			else if(c == '-')
			{
				res = a - b;
			}
			else if(c == '*')
			{
				res = a*b;
			}
			else if(c == '/')
			{
				res = a/b;
			}
			else
			{
				res = (float)pow((double)a, (double)b);
			}
			add_datof(nums, create_datof(res));
		}
		else
		{
			add_datof(nums, create_datof(strtof(d->s->s, NULL)));
		}
		d = get_dato(p);
	}
	return get_datof(nums)->d;
}

void stack_chunks(String* s, Pila* p)
{
	Pila* operadores = create_pila();
	Dato* d = NULL;
	int i;
	int count = 0;
	char* ns = NULL;
	String* s_ = NULL;
	int resultado_comparacion;
	char pc = '\0';
	char mult = '*';
	for(i = 0; i < s->length; i++)
	{
		if((s->s[i] >= '0' && s->s[i] <= '9') || s->s[i] == '.')
		{
			count++;
			ns = add_char_to_pointer(ns, count, s->s[i]);
		}
		else
		{
			if(s->s[i] == '(')
			{
				if(count > 0)
				{
					/*
					if('-' != ns[0] || ('-' == ns[0] && count > 1))
					{
					*/
						s_ = string(ns, count);
						ns = NULL;
						count = 0;
						d = create_dato(s_);
						add_dato(p, d);
					/*
					}
					else
					{
					
						count++;
						ns = add_char_to_pointer(ns, count, '1');
						add_dato(p, create_dato(string(ns, count)));
						ns = NULL;
						count = 0;
						d = get_dato(operadores);
						if(d)
						{
							resultado_comparacion = compare(mult, d->s->s[0]);
							if(resultado_comparacion == 3)
							{
								add_dato(operadores, d);
								add_dato_c(operadores, mult);
							}
							else if(resultado_comparacion <= 0)
							{
								while(resultado_comparacion <= 0)
								{
									add_dato(p, d);
									d = get_dato(operadores);
									if(!d || d->s->s[0] == '(')
										break;
									resultado_comparacion = compare(mult, d->s->s[0]);
								}
								if(d && d->s->s[0] != '(')
								{
									add_dato(operadores, d);
								}
								add_dato_c(operadores, mult);
							}
							else if(resultado_comparacion == 1)
							{
								add_dato(operadores, d);
								add_dato_c(operadores, mult);
							}
						}
						else
						{
							add_dato_c(operadores, mult);
						}
					}
					*/
				}
				add_dato_c(operadores, s->s[i]);
			}
			else if(s->s[i] == ')')
			{
				if(count > 0)
				{
					s_ = string(ns, count);
					ns = NULL;
					count = 0;
					d = create_dato(s_);
					add_dato(p, d);
				}
				// 4+5*(8-9/(8-1))-2
				/* Begin taking elements from "la pila de operadores", until you find an opening parentheses. */
				d = get_dato(operadores);
				while(d && d->s->s[0] != '(')
				{
					add_dato(p, d);
					d = get_dato(operadores);
				}
			}
			else
			{
				/*
				if(pc != '\0' && is_operator(pc) == 0 && s->s[i] == '-')
				{
				*/
					if(count > 0)
					{
						s_ = string(ns, count);
						ns = NULL;
						count = 0;
						d = create_dato(s_);
						add_dato(p, d);
					}
					/* Check priority */
					// Suma y Resta tienen la misma prioridad.
					// Multiplicación y División tienen la misma prioridad pero mayor que suma y resta.
					// Exponencial es de "derecha a izquierda", tiene más prioridad que suma y resta.
					// 5+3*2-18
					d = get_dato(operadores);
					if(d)
					{
						resultado_comparacion = compare(s->s[i], d->s->s[0]);
						if(resultado_comparacion > 0)
						{
							add_dato(operadores, d);
							add_dato_c(operadores, s->s[i]);
						}
						else // if(resultado_comparacion <= 0)
						{
							while(resultado_comparacion <= 0)
							{
								add_dato(p, d);
								d = get_dato(operadores);
								if(!d)
									break;
								resultado_comparacion = compare(s->s[i], d->s->s[0]);
							}
							if(d)
							{
								add_dato(operadores, d);
							}
							add_dato_c(operadores, s->s[i]);
						}
						/*
						else if(resultado_comparacion == 1)
						{
							add_dato(operadores, d);
							add_dato_c(operadores, s->s[i]);
						}
						*/
					}
					else
					{
						add_dato_c(operadores, s->s[i]);
					}
				//}
			}
		}
		pc = s->s[i];
	}
	if(ns)
	{
		add_dato(p, create_dato(string(ns, count)) );
		ns = NULL;
		count = 0;
	}
	dump_stack(operadores, p);
	free(operadores);
}

void dump_stack(Pila* source, Pila* target)
{
	Dato* d = get_dato(source);
	int i = 0;
	while(d)
	{
		if(d->s->s[0] != '(' && d->s->s[0] != ')')
		{
			add_dato(target, d);
		}
		d = get_dato(source);
		i++;
	}
}

int compare(char c1, char c2)
{
	int a = get_weight(c1);
	int b = get_weight(c2);
	return a-b;
}

int get_weight(char c)
{
	if( '+' == c || '-' == c )
	{
		return 1;
	}
	else if( '*' == c || '/' == c || '^' == c)
	{
		return 2;
	}
	return -1;
}

void add_dato(Pila* p, Dato* d)
{
	if(p && d)
	{
		if(!p->first)
		{
			p->first = d;
		}
		else
		{
			d->siguiente = p->first;
			p->first = d;
		}
	}
}

void add_dato_c(Pila* p, char c)
{
	char* pc = (char*) calloc(1, sizeof(char));
	pc[0] = c;
	String* s = string(pc, 1);
	Dato* d = create_dato(s);
	add_dato(p, d);
}

int is_operator(char c)
{
	char operators[] = {'+', '-', '*', '/', '^', '(', ')'};
	int res = char_in_valid_chars(c, operators);
	return res;
}

int char_in_valid_chars(char c, char* valid_chars)
{
	int i;
	for(i = 0; i < strlen(valid_chars); i++)
	{
		if(c == valid_chars[i])
			return 0;
	}
	return -1;
}

String* string(char* s, int length)
{
	String* str = (String*) calloc(1, sizeof(String));
	str->s = s;
	str->length = length;
	return str;
}

Dato* get_dato(Pila* p)
{
	if(p)
	{
		Dato* d = p->first;
		if(d)
		{
			p->first = d->siguiente;
			d->siguiente = NULL;
			return d;
		}
		else
			return NULL;
	}
	return NULL;
}

char* add_char_to_pointer(char* p, int count, char c)
{
	if(!p)
	{
		p = (char*) calloc(1, sizeof(char));
		p[0] = c;
	}
	else
	{
		p = (char*) reallocarray(p, count, sizeof(char));
		p[count-1] = c;
	}
	return p;
}

void highlight_problem(String* s, int position)
{
	int i;
	for(i = 0; i < s->length; i++)
	{
		if(i == position)
		{
			printf("-->%c<--", s->s[i]);
		}
		else
		{
			printf("%c", s->s[i]);
		}
	}
}

int validate_infix(String* s)
{
	char valid_chars[] = {'(', ')', '.', '+', '-', '*', '/', '^', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	char* ns = NULL;
	int index = 0;
	int i;
	int pc = 0;
	for(i = 0; i < s->length; i++)
	{
		if( s->s[i] == '(')
		{
			pc++;
			ns = add_char_to_pointer(ns, index + 1, s->s[i]);
			index++;
		}
		else if( s->s[i] == ')' )
		{
			pc--;
			ns = add_char_to_pointer(ns, index + 1, s->s[i]);
			index++;
		}
		else if(0 == char_in_valid_chars(s->s[i], valid_chars))
		{
			if(!ns)
			{
				ns = (char*) calloc(1, sizeof(char));
				ns[index] = s->s[i];
				index++;
			}
			else
			{
				ns = (char*) reallocarray(ns, 1 + index, sizeof(char));
				ns[index] = s->s[i];
				index++;
			}
		}
	}
	if(pc != 0)
	{
		return pc;
	}
	if(index > 0)
	{
		free(s->s);
		s->s = ns;
		s->length = index;
	}
	return 0;
}

int validate_decimal_numbers(String* s)
{
	int i;
	int pc = 0;
	for(i = 0; i < s->length; i++)
	{
		if( s->s[i] != '.' && !(s->s[i] >= '0' && s->s[i] <= '9') )
		{
			pc = 0;
		}
		else if( s->s[i] == '.')
		{
			if(pc > 0)
				return i;
			pc++;
		}
	}
	return 0;
}

int validate_syntax(String* s)
{
	if(s->s[0] == '.' || (is_operator(s->s[0]) == 0 && s->s[0] != '-' && s->s[0] != '('))
		return -1;
	int mc = 0;
	int i;
	char pc = '\0';
	int op = 0;
	for(i = 0; i < s->length; i++)
	{
		if( s->s[i] == '-' )
		{
			op++;
			mc++;
			if(mc > 2)
				return i;
		}
		else if( s->s[i] == ')' )
		{
			if(mc > 0 || ('\0' != pc && pc == '('))
				return i;
			mc = 0;
		}
		else if(is_operator(s->s[i]) == 0)
		{
			if(s->s[i] != '(' && s->s[i] != ')')
			{
				op++;
				if(op > 1)
					return i;
			}
			if(mc > 0 && s->s[i] != '(')
			{
				return i;
			}
			mc = 0;
		}
		else
		{
			op = 0;
			if(s->s[i] == '.' && mc > 0)
				return i;
			mc = 0;
		}
		pc = s->s[i];
	}
	if(( ')' != s->s[s->length - 1] && is_operator(s->s[s->length-1]) == 0) || s->s[s->length -1 ] == '.')
	{
		return s->length-1;
	}
	return 0;
}

int validate_string(String* s)
{
	char valid_chars[] = {'(', ')', '.', '+', '-', '*', '/', '^', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
	int i;
	char* ns = NULL;
	int count = 0;
	for(i = 0; i < s->length; i++)
	{
		if(0 == char_in_valid_chars(s->s[i], valid_chars))
		{
			count++;
			ns = add_char_to_pointer(ns, count, s->s[i]);
		}
		else
		{
			if(' ' != s->s[i])
			{
				printf("Cadena de texto ingresada incorrecta, se encontró: %c.\n", s->s[i]);
				if( i == 0 )
				{
					return -1;
				}
				if(ns)
				{
					free(ns);
				}
				return i;
			}
		}
	}
	s->s = ns;
	return 0;
}

String* read_file_contents(char* file_path)
{
	FILE* f = fopen(file_path, "r");
	if(f)
	{
		char* c = (char*) calloc(1, sizeof(char));
		char* s = (char*) calloc(1, sizeof(char));
		int count = 0;
		int cr = 0;
		do
		{
			cr = fread(c, sizeof(char), 1, f);
			count += cr;
			if(*c == '\n')
			{
				count--;
				break;
			}
			else
			{
				if(count == 1)
				{
					s[0] = *c;
				}
				else
				{
					s = (char*) reallocarray(s, count, sizeof(char));
					s[count-1] = *c;
				}
			}
		}while(cr != 0);
		free(c);
		return string(s, count);
	}
	else
	{
		return NULL;
	}
}

Pila* create_pila()
{
	Dato* first = NULL;
	Pila* pila = (Pila*) malloc(sizeof(Pila));
	pila->first = NULL;
	return pila;
}

Dato* create_dato(String* s)
{
	Dato* dato = (Dato*)malloc(sizeof(Dato));
	dato->s = s;
	dato->siguiente = NULL;
	return dato;
}
