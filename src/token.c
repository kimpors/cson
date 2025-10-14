#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void print_tok(Token *tok)
{
	switch (tok->type)
	{
		case BRACKET:
			puts("type: bracket");
			printf("value: %c\n", (char)(long)tok->value);
			break;
		case SEPARATOR:
			puts("type: separator");
			printf("value: %c\n", (char)(long)tok->value);
			break;
		case COMA:
			puts("type: coma");
			printf("value: %c\n", (char)(long)tok->value);
			break;
		case STRING:
			puts("type: string");
			printf("value: %s\n", (char *)tok->value);
			break;
		case NUMBER:
			puts("type: number");
			printf("value: %lf\n", *(double *)tok->value);
			break;
		case BOOL:
			puts("type: bool");
			printf("value: %s\n", (long)tok->value > 0 ? "true" : "false");
			break;
		case NIL:
			puts("type: nil");
			printf("value: null\n");
			break;
		default:
			break;
	}
}

Token *push(TokenType type, void *value);

char *gettoken(char *s, Token *dest, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;

	char *ps = s;

	if (*s == '{' || *s == '}' ||
		*s == '[' || *s == ']')
	{
		dest->type = BRACKET;
		dest->value = (void *)(long)*s;
		s++;
	}
	else if (*s == ':')
	{
		dest->type = SEPARATOR;
		dest->value = (void *)(long)*s;
		s++;
	}
	else if (*s == ',')
	{
		dest->type = COMA;
		dest->value = (void *)(long)*s;
		s++;
	}
	else if (isdigit(*s))
	{
		ps = s;
		while (isdigit(*++ps));

		int c = *ps;
		*ps = '\0';

		dest->type = NUMBER;
		dest->value = malloc(sizeof(double));
		*((double *)dest->value) = atof(s);

		*ps = c;
		s = ps;
	}
	else if (*s == '"' && s++)
	{
		ps = s;
		while (*ps != '\0' && *++ps != '"');

		dest->type = STRING;
		dest->value = malloc(ps - s);
		strncpy(dest->value, s, ps - s);
		s = ps + 1;
	}
	else if (!strncmp(s, "null", 4))
	{
		dest->type = NIL;
		dest->value = NULL;
		s += 5;
	}
	else if (!strncmp(s, "true", 4))
	{
		dest->type = BOOL;
		dest->value = (void *)true;
		s += 5;
	}
	else if (!strncmp(s, "false", 5))
	{
		dest->type = BOOL;
		dest->value = false;
		s += 6;
	}
	else
	{
		dest->type = NONE;
		dest->value = 0;
		s++;
	}

	return s;
}

#define MAX_SIZE 128

struct {
	size_t i;
	Token data[MAX_SIZE];
} buf;

Token *push(TokenType type, void *value)
{
	if (buf.i > MAX_SIZE - 1) return NULL;
	buf.data[buf.i].type = type;
	buf.data[buf.i].value = value;
	return &buf.data[buf.i++];
}
