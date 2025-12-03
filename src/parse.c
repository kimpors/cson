#include <stdio.h>
#include "jarray.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

void jprintobj(JObject *obj)
{
	for (size_t i = 0; i < jlen(obj); i++)
	{
		printf("[KEY: %10s]\t", obj[i].key);

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
				jprintobj(obj[i].val.value.obj);
				puts("[OBJECT END]");
				break;
			case ARRAY:
				printf("[TYPE: %10s]\n", "array");
				puts("[ARRAY BEGIN]");
				jprintobj(obj[i].val.value.obj);
				puts("[ARRAY END]");
				break;
			}
        }
}

JObject *jparse(JToken *toks)
{
	size_t j = 0;
	char brack = '}';
	bool issep = false;

	JObject tmp = { 0 };
	JObject *res = NULL;

	for (size_t i = 1, j = 0; i < jlen(toks); i++)
	{
		switch (toks[i].type)
		{
			case COMA: 
				issep = false;
				jpush(res, tmp);
				tmp = (JObject){ 0 };
				break;
			case SEPARATOR: issep = true; break;
			case BRACKET:
				if (toks[i].str[0] == ']' ||
					toks[i].str[0] == '}')
				{
					jpush(res, tmp);
					continue;
				}
						
				if (toks[i].str[0] == '[')
				{
					brack = ']';
				}

				for (j = i; j < jlen(toks); j++)
				{
					if (toks[j].type == BRACKET &&
						toks[j].str[0] == brack)
					{
						break;
					}
				}

				if (brack == '{')
				{
					tmp.val = ((JValue){ OBJECT, .value.obj = jparse(toks + i) });
				}

				i = j;
				break;
			case VALUE:
				if (!issep)
				{
					tmp.key = toks[i].str + 1;
					continue;
				}

				if (*toks[i].str == '"')
				{
					jinit(tmp.val.value.str, jlen(toks[i].str));
					jlen(tmp.val.value.str) = jlen(toks[i].str) - 1;
					strncpy(tmp.val.value.str, toks[i].str + 1, jlen(toks[i].str) - 1);
					tmp.val.value.str[jlen(tmp.val.value.str)] = '\0';
					tmp.val.type = STRING;
				}
				else if (isdigit(*toks[i].str))
				{
					tmp.val = ((JValue){ NUMBER, .value.num = atof(toks[i].str)} );
				}
				else if (!strncmp(toks[i].str, "null", 4))
				{
					tmp.val = ((JValue){ NIL, 0 });
				}
				else if (!strncmp(toks[i].str, "true", 4))
				{
					tmp.val = ((JValue){ BOOL, .value.boo = true });
				}
				else if (!strncmp(toks[i].str, "false", 5))
				{
					tmp.val = ((JValue){ BOOL, .value.boo = false });
				}
				break;
		}
	}

	return res;
}
