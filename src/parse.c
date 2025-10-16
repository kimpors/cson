#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

void print_value(JValue *arr)
{
	switch (arr->type) 
	{
        case NIL:
			puts("type: nil");
			break;
        case STRING:
			puts("type: string");
			printf("value: %s\n", arr->value.str);
			break;
        case NUMBER:
			puts("type: number");
			printf("value: %lf\n", arr->value.num);
			break;
        case BOOL:
			puts("type: bool");
			printf("value: %s\n", !arr->value.boo ? "false" : "true");
			break;
		case OBJECT:
			puts("type: object");
			puts("object begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_item(&arr->value.obj[i]);
			}
			puts("object end");
			break;
		case ARRAY:
			puts("type: array");
			puts("array begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_value(&arr->value.arr[i]);
			}
			puts("array end");
			break;
    }
}

void print_item(JItem *obj)
{
	printf("key: %s\n", obj->key);

	switch (obj->type) 
	{
        case NIL:
			puts("type: nil");
			break;
        case STRING:
			puts("type: string");
			printf("value: %s\n", obj->value.str);
			break;
        case NUMBER:
			puts("type: number");
			printf("value: %lf\n", obj->value.num);
			break;
        case BOOL:
			puts("type: bool");
			printf("value: %s\n", !obj->value.boo ? "false" : "true");
			break;
		case OBJECT:
			puts("type: object");
			puts("object begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_item( &obj->value.obj[i]);
			}
			puts("object end");
			break;
		case ARRAY:
			puts("type: array");
			puts("array begin");
			for (size_t i = 0; i < 6; i++)
			{
				print_value(&obj->value.arr[i]);
			}
			puts("array end");
			break;
    }
}

#define MAX_BUF	255

JValue *parse_array(JToken *toks, size_t lim)
{
	size_t temp;
	JValue *buf = malloc(sizeof(JValue) * MAX_BUF);

	for (size_t i = 1, j = 0; i < lim; i++)
	{
		switch (toks[i].type)
		{
			case COMA:
				j++;
				break;
			case BRACKET:
				if (*toks[i].value == '{')
				{
					for (temp = i; temp < lim; temp++)
					{
						if (toks[temp].type == BRACKET &&
							(long)toks[temp].value == '}')
						{
							break;
						}
					}

					buf[j].type = OBJECT;
					buf[j].value.obj = parse(toks + i, temp - i);
					i = temp;
				}
				else if (*toks[i].value == '[')
				{
					for (temp = i; temp < lim; temp++)
					{
						if (toks[temp].type == BRACKET &&
							(long)toks[temp].value == ']')
						{
							break;
						}
					}

					buf[j].type = ARRAY;
					buf[j].value.arr = parse_array(toks + i, temp - i);
					i = temp;
				}
				break;
			case VALUE:
					if (*toks[i].value == '"')
					{
						buf[j].type = STRING;
						buf[j].value.str = toks[i].value + 1;
					}
					else if (isdigit(*toks[i].value))
					{
						buf[j].type = NUMBER;
						buf[j].value.num = atof(toks[i].value);
					}
					else if (!strncmp(toks[i].value, "null", 4))
					{
						buf[j].type = NIL;
						buf[j].value.boo = NULL;
					}
					else if (!strncmp(toks[i].value, "true", 4))
					{
						buf[j].type = BOOL;
						buf[j].value.boo = true;
					}
					else if (!strncmp(toks[i].value, "false", 5))
					{
						buf[j].type = BOOL;
						buf[j].value.boo = false;
					}
				break;
			default:
				fprintf(stderr, "parse_array: wrong format <|-_-|>\n");
				return NULL;
				break;
		}
	}

	return buf;
}

JItem *parse(JToken *toks, size_t lim)
{
	size_t temp = 0;
	bool issep = false;
	JItem *buf = malloc(sizeof(JItem) * MAX_BUF);

	for (size_t i = 1, j = 0; i < lim; i++)
	{
		if (toks[i].type == BRACKET)
		{
			switch ((long)toks[i].value) 
			{
				case '{':
					for (temp = i; temp < lim; temp++)
					{
						if (toks[temp].type == BRACKET &&
							(long)toks[temp].value == '}')
						{
							break;
						}
					}

					buf[j].type = OBJECT;
					buf[j].value.obj = parse(toks + i, temp - i);
					i = temp;
					break;
				case '[':
					for (temp = i; temp < lim; temp++)
					{
						if (toks[temp].type == BRACKET &&
							(long)toks[temp].value == ']')
						{
							break;
						}
					}

					buf[j].type = ARRAY;
					buf[j].value.arr = parse_array(toks + i, temp - i);
					i = temp;
					break;
			}
		}
		else if (toks[i].type == COMA)
		{
			j++;
		}
		else if (toks[i].type == SEPARATOR)
		{
			issep = true;
		}
		else if (toks[i].type == VALUE)
		{
			if (!issep)
			{
				buf[j].key = toks[i].value + 1;
				continue;
			}

			if (*toks[i].value == '"')
			{
				buf[j].type = STRING;
				buf[j].value.str = toks[i].value + 1;
			}
			else if (isdigit(*toks[i].value))
			{
				buf[j].type = NUMBER;
				buf[j].value.num = atof(toks[i].value);
			}
			else if (!strncmp(toks[i].value, "null", 4))
			{
				buf[j].type = NIL;
				buf[j].value.boo = NULL;
			}
			else if (!strncmp(toks[i].value, "true", 4))
			{
				buf[j].type = BOOL;
				buf[j].value.boo = true;
			}
			else if (!strncmp(toks[i].value, "false", 5))
			{
				buf[j].type = BOOL;
				buf[j].value.boo = false;
			}

			issep = false;
		}
	}

	return buf;
}
