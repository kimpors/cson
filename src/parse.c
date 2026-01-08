#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "jarray.h"
#include "jerror.h"
#include "jparse.h"
#include "jtoken.h"

void jitemprint(JItem *items)
{
	if (!items) 
	{
		JERROR_MSG("'items' is uninitilized");
		return;
	}

	for (size_t i = 0; i < jlen(items); i++)
	{
		switch (items[i].type)
		{
			case NIL:
				if (items[i].key) printf("[KEY: %10s]\t", items[i].key);
				else printf("[TYPE: %10s]\n", "nil");
				break;
			case STRING:
				if (items[i].key) printf("[KEY: %10s]\t", items[i].key);
				else printf("[TYPE: %10s]\t[VALUE: %s]\n", "string", items[i].value.str);
				break;
			case NUMBER:
				if (items[i].key) printf("[KEY: %10s]\t", items[i].key);
				else printf("[TYPE: %10s]\t[VALUE: %lf]\n", "number", items[i].value.num);
				break;
			case BOOL:
				if (items[i].key) printf("[KEY: %10s]\t", items[i].key);
				else printf("[TYPE: %10s]\t[VALUE: %s]\n", "bool", !items[i].value.boo ? "false" : "true");
				break;
			case OBJECT:
				printf("[KEY: %10s]\t", items[i].key);
				printf("[TYPE: %10s]\n", "object");
				puts("[OBJECT BEGIN]");
				jitemprint(items[i].value.arr);
				puts("[OBJECT END]");
				break;
			case ARRAY:
				printf("[KEY: %10s]\t", items[i].key);
				printf("[TYPE: %10s]\n", "array");
				puts("[ARRAY BEGIN]");
				jitemprint(items[i].value.arr);
				puts("[ARRAY END]");
				break;
			default:
				JWARN_MSG("'items[i]' is not specified (type: %u, value: %.2f)", items[i].type, items[i].value.num);
				break;
			}
        }
}

JItem *jitemparse(JToken *toks)
{
	if (!toks) 
	{
		JERROR_MSG("'toks' is unitilized");
		return NULL;
	}

	JItem tmp = { 0 };
	JItem *res = NULL;

	bool issep = false;
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

				j = i + 1;

				for (size_t k = 0; j < jlen(toks); j++)
				{
					if (toks[j].type == BRACKET &&
						*toks[j].str == begin)
					{
						k++;
					}

					if (toks[j].type == BRACKET &&
						*toks[j].str == end && k-- == 0)
					{
						break;
					}
				}

				switch (end)
				{
					case '}':
						tmp.type = OBJECT;
						tmp.value.arr = jitemparse(toks + i);
						break;
					case ']':
						tmp.type = ARRAY;
						tmp.value.arr = jitemparse(toks + i);
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
					jstrncpy(tmp.value.str, toks[i].str + 1, jlen(toks[i].str) - 1);
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

char *jitemtojson(JItem *items, size_t tabs, bool strip)
{
	if (!items) return NULL;

	char sbuf[20];
	char *res = NULL, *tmp = NULL;

	if (!items->key) 
	{
		jconcat(res, "[");
	}
	else 
	{
		jconcat(res, "{"); 
	}

	if (!strip && items->key != NULL) { jconcat(res, "\n"); }

	for (size_t i = 0; i < jlen(items); i++)
	{
		if (items->key)
		{
			for (size_t j = 0; j < tabs; j++)
			{
				jconcat(res, "\t");
			}

			jconcat(res, "\t\"");
			jconcat(res, items[i].key);
			jconcat(res, "\"");

			if (strip) { jconcat(res, ":"); }
			else { jconcat(res, " : "); }
		}

		switch(items[i].type)
		{
			case NIL:
				jconcat(res, "null");
				break;
			case STRING:
				jconcat(res, "\"");
				jconcat(res, items[i].value.str);
				jconcat(res, "\"");
				break;
			case NUMBER:
				if (items[i].value.num == trunc(items[i].value.num)) sprintf(sbuf, "%.0f", items[i].value.num);
				else sprintf(sbuf, "%.1f", items[i].value.num);
				jconcat(res, sbuf);
				break;
			case BOOL:
				jconcat(res, items[i].value.boo ? "true" : "false");
				break;
			case ARRAY:
			case OBJECT:
				tmp = jitemtojson(items[i].value.arr, tabs + 1, strip);
				jconcat(res, tmp);
				jfree(tmp);
				tmp = NULL;
			  break;
        }

		if (!strip && !items->key && i < jlen(items) - 1) { jconcat(res, ", "); }
		else if (i < jlen(items) - 1) { jconcat(res, ","); }

		if (!strip && items->key != NULL) { jconcat(res, "\n"); }
	}

	if (!items->key) { jconcat(res, "]"); }
	else 
	{
		for (size_t j = 0; j < tabs; j++)
		{
			jconcat(res, "\t");
		}

		jconcat(res, "}"); 
	}

	return res;
}
