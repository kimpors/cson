#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parse.h"
#include "token.h"

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
		default:
          break;
    }
}

#define MAX_BUF	255

JItem *parse(JToken *toks, size_t lim)
{
	size_t i = 0;
	bool issep = false;
	JItem *buf = malloc(sizeof(JItem) * MAX_BUF);

	for (size_t i = 0, j = 0; i < lim; i++)
	{
		if (toks[i].type == COMA)
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
