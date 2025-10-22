#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void jprinttok(JToken *restrict tok)
{
	switch (tok->type)
	{
		case BRACKET:
			printf("[TYPE: %9s]\t[VALUE: '%c']\n", "bracket", (char)(long)tok->value);
			break;
		case SEPARATOR:
			printf("[TYPE: %9s]\n", "separator");
			break;
		case COMA:
			printf("[TYPE: %9s]\n", "coma");
			break;
		case VALUE:
			printf("[TYPE: %9s]\t[VALUE: '%s']\n", "value", (char *)tok->value);
			break;
	}
}

char *jgettoken(char *s, JToken *restrict dest, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;
	if (!*s) return NULL;

	char *ps = s;

	switch (*s)
	{
		case '{': case '}':
		case '[': case ']':
			dest->type = BRACKET;
			dest->value = (void *)(long)*s;
			s++;
			break;
		case ':':
			dest->type = SEPARATOR;
			s++;
			break;
		case ',':
			dest->type = COMA;
			s++;
			break;
		case '"':
			ps = s;
			while (*ps != '\0' && *++ps != '"');

			dest->type = VALUE;
			dest->value = malloc(ps - s);
			strncpy(dest->value, s, ps - s);
			s = ps + 1;
			break;
		default:
			ps = s;
			while (isalnum(*++ps));

			dest->type = VALUE;
			dest->value = malloc(ps - s);
			strncpy(dest->value, s, ps - s);
			s = ps;
			break;
	}

	return s;
}
