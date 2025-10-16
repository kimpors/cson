#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

void print_value(JValue *val)
{
	switch (val->type) 
	{
        case NIL:
			puts("type: nil");
			printf("value: %ld\n", JNIL(val->value));
			break;
        case STRING:
			puts("type: string");
			printf("value: %s\n", JSTR(val->value));
			break;
        case NUMBER:
			puts("type: number");
			printf("value: %lf\n", JNUM(val->value));
			break;
        case BOOL:
			puts("type: bool");
			printf("value: %s\n", !JBOO(val->value) ? "false" : "true");
			break;
		case OBJECT:
			puts("type: object");
			puts("object begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_item(&((JItem *)val->value)[i]);
			}
			puts("object end");
			break;
		case ARRAY:
			puts("type: array");
			puts("array begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_value(&((JValue *)val->value)[i]);
			}
			puts("array end");
			break;
    }
}

void print_item(JItem *item)
{
	printf("key: %s\n", item->key);

	switch (item->type) 
	{
        case NIL:
			puts("type: nil");
			printf("value: %ld\n", JNIL(item->value));
			break;
        case STRING:
			puts("type: string");
			printf("value: %s\n", JSTR(item->value));
			break;
        case NUMBER:
			puts("type: number");
			printf("value: %lf\n", JNUM(item->value));
			break;
        case BOOL:
			puts("type: bool");
			printf("value: %s\n", !JBOO(item->value) ? "false" : "true");
			break;
		case OBJECT:
			puts("type: object");
			puts("object begin");
			for (size_t i = 0; i < 2; i++)
			{
				print_item(&((JItem *)item->value)[i]);
			}
			puts("object end");
			break;
		case ARRAY:
			puts("type: array");
			puts("array begin");
			for (size_t i = 0; i < 6; i++)
			{
				print_value(&((JValue *)item->value)[i]);
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
					buf[j].value = (void *)parse(toks + i, temp - i);
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
					buf[j].value = (void *)parse_array(toks + i, temp - i);
					i = temp;
				}
				break;
			case VALUE:
					if (*toks[i].value == '"')
					{
						buf[j].type = STRING;
						buf[j].value = toks[i].value + 1;
					}
					else if (isdigit(*toks[i].value))
					{
						buf[j].type = NUMBER;
						double *temp = malloc(sizeof(double));
						*temp = atof(toks[i].value);
						buf[j].value = (void *)temp;
					}
					else if (!strncmp(toks[i].value, "null", 4))
					{
						buf[j].type = NIL;
						buf[j].value = NULL;
					}
					else if (!strncmp(toks[i].value, "true", 4))
					{
						buf[j].type = BOOL;
						buf[j].value = (void *)true;
					}
					else if (!strncmp(toks[i].value, "false", 5))
					{
						buf[j].type = BOOL;
						buf[j].value = (void *)false;
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
					buf[j].value = (void *)parse(toks + i, temp - i);
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
					buf[j].value = (void *)parse_array(toks + i, temp - i);
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
				buf[j].value = toks[i].value + 1;
			}
			else if (isdigit(*toks[i].value))
			{
				buf[j].type = NUMBER;
				double *temp = malloc(sizeof(double));
				*temp = atof(toks[i].value);
				buf[j].value = (void *)temp;
			}
			else if (!strncmp(toks[i].value, "null", 4))
			{
				buf[j].type = NIL;
				buf[j].value = NULL;

			}
			else if (!strncmp(toks[i].value, "true", 4))
			{
				buf[j].type = BOOL;
				buf[j].value = (void *)true;
			}
			else if (!strncmp(toks[i].value, "false", 5))
			{
				buf[j].type = BOOL;
				buf[j].value = (void *)false;
			}

			issep = false;
		}
	}

	return buf;
}
