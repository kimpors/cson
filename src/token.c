#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void print_tok(JToken *tok)
{
	switch (tok->type)
	{
		case BRACKET:
			puts("type: bracket");
			printf("value: %c\n", (char)(long)tok->value);
			break;
		case SEPARATOR:
			puts("type: separator");
			break;
		case COMA:
			puts("type: coma");
			break;
		case VALUE:
			puts("type: value");
			printf("value: %s\n", (char *)(long)tok->value);
			break;
	}
}

char *gettoken(char *s, JToken *dest, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;

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
			while (*ps != '\0' && isalnum(*++ps));

			dest->type = VALUE;
			dest->value = malloc(ps - s);
			strncpy(dest->value, s, ps - s);
			s = ps + 1;
			break;
	}

	return s;
}
