#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "parse.h"
#include "token.h"

void jprintobj(JObject *obj, bool isobj)
{
	for (size_t i = 0; i < obj->size; i++)
	{
		if (isobj)
		{
			printf("[KEY: %10s]\n", obj->keys[i]);
		}

		switch (obj->values[i].type)
		{
			case NIL:
				printf("[TYPE: %10s]\n", "nil");
				break;
			case STRING:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "string", obj->values[i].value.str);
				break;
			case NUMBER:
				printf("[TYPE: %10s]\t[VALUE: %lf]\n", "number", obj->values[i].value.num);
				break;
			case BOOL:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "bool", !obj->values[i].value.boo ? "false" : "true");
				break;
			case OBJECT:
				printf("[TYPE: %10s]\n", "object");
				puts("[OBJECT BEGIN]");
				jprintobj(obj->values[i].value.obj, true);
				puts("[OBJECT END]");
				break;
			case ARRAY:
				printf("[TYPE: %10s]\n", "array");
				puts("[ARRAY BEGIN]");
				jprintobj(obj->values[i].value.obj, false);
				puts("[ARRAY END]");
				break;
			}
        }
}

#define MAX_BUF	128

JObject *jparse(JToken *toks, size_t lim, bool isobj)
{
	size_t j = 0;
	char brack = '}';
	bool issep = false;

	JObject *buf = malloc(sizeof(JObject));
	buf->keys = calloc(MAX_BUF, sizeof(char *));
	buf->values = calloc(MAX_BUF, sizeof(JValue));

	JValue *pval = &buf->values[j];

	for (size_t i = 1, temp = 0; i < lim; i++)
	{
		switch (toks[i].type)
		{
			case COMA: 
				pval = &buf->values[++j];
				break;
			case SEPARATOR: issep = true; break;
			case BRACKET:
				if ((long) toks[i].value == ']' ||
					(long) toks[i].value == '}')
				{
					continue;
				}
						
				if ((long) toks[i].value == '[')
				{
					brack = ']';
				}

				for (temp = i; temp < lim; temp++)
				{
					if (toks[temp].type == BRACKET &&
						(long)toks[temp].value == brack)
					{
						break;
					}
				}

				buf->size++;
				pval->type = brack == '{' ? OBJECT : ARRAY;
				pval->value.obj = jparse(toks + i, temp - i, pval->type == OBJECT);
				i = temp;
				break;
			case VALUE:
				if (isobj && !buf->keys[j])
				{
					buf->keys[j] = toks[i].value + 1;
					continue;
				}

				buf->size++;

				if (*toks[i].value == '"')
				{
					pval->type = STRING;
					pval->value.str = toks[i].value + 1;
				}
				else if (isdigit(*toks[i].value))
				{
					pval->type = NUMBER;
					pval->value.num = atof(toks[i].value);
				}
				else if (!strncmp(toks[i].value, "null", 4))
				{
					pval->type = NIL;
					pval->value.boo = NULL;
				}
				else if (!strncmp(toks[i].value, "true", 4))
				{
					pval->type = BOOL;
					pval->value.boo = true;
				}
				else if (!strncmp(toks[i].value, "false", 5))
				{
					pval->type = BOOL;
					pval->value.boo = false;
				}
				break;
		}
	}

	return buf;
}
