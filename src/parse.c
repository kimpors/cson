#include <stdio.h>
#include "jarray.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

void jprint(JItem *items)
{
	for (size_t i = 0; i < jlen(items); i++)
	{
		switch (items[i].type)
		{
			case NIL:
				printf("[TYPE: %10s]\n", "nil");
				break;
			case STRING:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "string", items[i].value.str);
				break;
			case NUMBER:
				printf("[TYPE: %10s]\t[VALUE: %lf]\n", "number", items[i].value.num);
				break;
			case BOOL:
				printf("[TYPE: %10s]\t[VALUE: %s]\n", "bool", !items[i].value.boo ? "false" : "true");
				break;
			case OBJECT:
				printf("[KEY: %10s]\t", items[i].key);
				printf("[TYPE: %10s]\n", "object");
				puts("[OBJECT BEGIN]");
				jprint(items[i].value.arr);
				puts("[OBJECT END]");
				break;
			case ARRAY:
				printf("[KEY: %10s]\t", items[i].key);
				printf("[TYPE: %10s]\n", "array");
				puts("[ARRAY BEGIN]");
				jprint(items[i].value.arr);
				puts("[ARRAY END]");
				break;
			}
        }
}

JItem *jparse(JToken *toks)
{
	size_t j = 0;
	JItem tmp = { 0 };
	JItem *res = NULL;

	bool issep = false;
	char brack = *toks->str;
	char begin = *toks->str;
	char end;

	for (size_t i = 1, j = 0; i < jlen(toks); i++)
	{
		switch (toks[i].type)
		{
			case SEPARATOR: 
				issep = true; 
				break;
			case COMA: 
				issep = false;
				jpush(res, tmp);
				tmp = (JItem){ 0 };
				break;
			case BRACKET:
				switch (*toks[i].str)
				{
					case '{':
						end = '}';
						break;
					case '[':
						end = ']';
						break;
					case '}':
					case ']':
						jpush(res, tmp);
						return res;
				}

				for (j = i; j < jlen(toks); j++)
				{
					if (toks[j].type == BRACKET &&
						*toks[j].str == end)
					{
						break;
					}
				}

				switch (end)
				{
					case '}':
						tmp.type = OBJECT;
						tmp.value.arr = jparse(toks + i);
						break;
					case ']':
						tmp.type = ARRAY;
						tmp.value.arr = jparse(toks + i);
						break;
				}

				i = j;
				break;
			case VALUE:
				if (!issep && begin == '{')
				{
					tmp.key = toks[i].str + 1;
					continue;
				}

				if (*toks[i].str == '"')
				{
					jinit(tmp.value.str, jlen(toks[i].str));
					jlen(tmp.value.str) = jlen(toks[i].str) - 1;
					strncpy(tmp.value.str, toks[i].str + 1, jlen(toks[i].str) - 1);
					tmp.value.str[jlen(tmp.value.str)] = '\0';
					tmp.type = STRING;
				}
				else if (isdigit(*toks[i].str))
				{
					tmp.type = NUMBER;
					tmp.value.num = atof(toks[i].str);
				}
				else if (!strncmp(toks[i].str, "null", 4))
				{
					tmp.type = NIL;
					tmp.value.num = 0;
				}
				else if (!strncmp(toks[i].str, "true", 4))
				{
					tmp.type = BOOL;
					tmp.value.boo = true;
				}
				else if (!strncmp(toks[i].str, "false", 5))
				{
					tmp.type = BOOL;
					tmp.value.boo = false;
				}
				break;
		}
	}

	return res;
}
