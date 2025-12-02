#include <stdio.h>
#include "jarray.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

void jprintobj(JObject *obj, bool isobj)
{
	for (size_t i = 0; i < jlen(obj); i++)
	{
		if (isobj)
		{
			printf("[KEY: %10s]\n", obj[i].key);
		}

		switch (obj[i].val.type)
		{
			case NIL:
				printf("[TYPE: %10s]\n", "nil");
				break;
			case STRING:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "string", obj[i].val.value.str);
				break;
			case NUMBER:
				printf("[TYPE: %10s]\t[VALUE: %lf]\n", "number", obj[i].val.value.num);
				break;
			case BOOL:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "bool", !obj[i].val.value.boo ? "false" : "true");
				break;
			case OBJECT:
				printf("[TYPE: %10s]\n", "object");
				puts("[OBJECT BEGIN]");
				jprintobj(obj[i].val.value.obj, true);
				puts("[OBJECT END]");
				break;
			case ARRAY:
				printf("[TYPE: %10s]\n", "array");
				puts("[ARRAY BEGIN]");
				jprintobj(obj[i].val.value.obj, false);
				puts("[ARRAY END]");
				break;
			}
        }
}

JObject *jparse(JToken *toks, size_t lim, bool isobj)
{
	size_t len = 1;
	size_t j = 0;
	char brack = '}';
	bool issep = false;

	for (size_t i = 1; i < lim; i++)
	{
		if (toks[i].type == COMA) len++;
	}

	JObject *buf = NULL;
	jinit(buf, len);

	JValue *pval = &buf[j].val;

	for (size_t i = 1, temp = 0; i < lim; i++)
	{
		switch (toks[i].type)
		{
			case COMA: 
				pval = &buf[++j].val;
				break;
			case SEPARATOR: issep = true; break;
			case BRACKET:
				if ((long) toks[i].str == ']' ||
					(long) toks[i].str == '}')
				{
					continue;
				}
						
				if ((long) toks[i].str == '[')
				{
					brack = ']';
				}

				for (temp = i; temp < lim; temp++)
				{
					if (toks[temp].type == BRACKET &&
						(long)toks[temp].str == brack)
					{
						break;
					}
				}

				jlen(buf)++;
				pval->type = brack == '{' ? OBJECT : ARRAY;
				pval->value.obj = jparse(toks + i, temp - i, pval->type == OBJECT);
				i = temp;
				break;
			case VALUE:
				if (isobj && !buf[j].key)
				{
					buf[j].key = toks[i].str + 1;
					continue;
				}

				jlen(buf)++;

				if (*toks[i].str == '"')
				{
					char *temp = NULL;
					jinit (temp, jlen(toks[i].str + 1));
					strncpy(temp, toks[i].str, jlen(toks[i].str));
					temp[jlen(temp)] = '\0';


					// JToken *temp = NULL;
					// jinit(temp, jlen(toks[i].str + 1));
					// strncpy(temp[0].str, toks[i].str, jlen(toks[i].str));
					// temp[0].str[jlen(temp[0].str)] = '\0';
					jpush(pval, ((JValue){ STRING, .value.str = temp }));



					// pval->type = STRING;
					// jstrset(&pval->value.str, toks[i].str.value, toks[i].str.size);
				}
				else if (isdigit(*toks[i].str))
				{
					pval->type = NUMBER;
					pval->value.num = atof(toks[i].str);
				}
				else if (!strncmp(toks[i].str, "null", 4))
				{
					pval->type = NIL;
					pval->value.boo = NULL;
				}
				else if (!strncmp(toks[i].str, "true", 4))
				{
					pval->type = BOOL;
					pval->value.boo = true;
				}
				else if (!strncmp(toks[i].str, "false", 5))
				{
					pval->type = BOOL;
					pval->value.boo = false;
				}
				break;
		}
	}

	return buf;
}
