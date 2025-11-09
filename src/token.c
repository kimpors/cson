#include "token.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void jtokprint(JToken *restrict tok)
{
	switch (tok->type)
	{
		case BRACKET:
			printf("[TYPE: %9s]\t[VALUE: '%s']\n", "bracket", tok->value);
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

void jtoksprint(JTokens *toks)
{
	printf("[SIZE: %4ld][CAPACITY: %4ld]\n", toks->size, toks->capacity);
	printf("====================================\n");

	for (size_t i = 0; i < toks->size; i++)
	{
		jtokprint(toks->toks + i);
	}
}

char *jgettoken(char *s, JTokens *dest, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;
	if (!*s) return NULL;

	char *ps;
	switch (*s)
	{
		case '{': case '}':
		case '[': case ']':
			jtokspush(dest, (JToken){ s, BRACKET, 1});
			s++;
			break;
		case ':':
			jtokspush(dest, (JToken){ .type = SEPARATOR });
			s++;
			break;
		case ',':
			jtokspush(dest, (JToken){ .type = COMA });
			s++;
			break;
		case '"':
			ps = s;
			while (*ps != '\0' && *++ps != '"');
			jtokspush(dest, (JToken){ s, VALUE, ps - s });
			s = ps + 1;
			break;
		default:
			ps = s;
			while (isalnum(*++ps));
			jtokspush(dest, (JToken){ s, VALUE, ps - s });
			s = ps;
			break;
	}

	return s;
}

char *jtokinit(JToken *dest, size_t capacity)
{
	if (!dest || !capacity) return NULL;

	dest->size = 0;
	dest->capacity = capacity;

	if (!(dest->value = calloc(capacity, sizeof(char))))
	{
		fprintf(stderr, "can't init token\n");
		return NULL;
	}
	
	return dest->value;
}

char *jtokexpand(JToken *dest, size_t capacity)
{
	if (!dest || !capacity ||
		dest->capacity >= capacity) return NULL;

	if (!(dest->value = realloc(dest->value, capacity * sizeof(char))))
	{
		fprintf(stderr, "can't expand token\n");
		return NULL;
	}
			
	return dest->value;
}

char *jtokset(JToken *dest, const char *s, size_t size)
{
	if (!dest) return NULL;
	if (!dest->capacity) 
	{
		jtokinit(dest, size + 1);
	}
	if (size >= dest->capacity &&
			!jtokexpand(dest, (dest->capacity + size) * 2))
	{
		fprintf(stderr, "can't set token\n");
		return NULL;
	}

	strncpy(dest->value, s, size);
	return dest->value;
}

JToken *jtoksinit(JTokens *dest, size_t capacity)
{
	if (!dest || !capacity) return NULL;

	dest->size = 0;
	dest->capacity = capacity;
	if (!(dest->toks = calloc(capacity, sizeof(JToken))))
	{
		fprintf(stderr, "cant init tokens\n");
		return NULL;
	}

	return dest->toks;
}

JToken *jtoksexpand(JTokens *dest, size_t capacity)
{
	if (!dest || !capacity || 
		dest->capacity >= capacity) return NULL;

	dest->capacity = capacity;

	if (!(dest->toks = realloc(dest->toks, capacity * sizeof(JToken))))
	{
		fprintf(stderr, "can't expand tokens\n");
		return NULL;
	}

	for (size_t i = dest->size; i < dest->capacity; i++)
	{
		dest->toks[i].size = 0;
		dest->toks[i].capacity = 0;
		dest->toks[i].value = NULL;
	}

	return dest->toks;
}

JToken *jtokspush(JTokens *dest, JToken tok)
{
	if (!dest) return NULL;
	if (dest->size >= dest->capacity &&
			!jtoksexpand(dest, dest->capacity * 2))
	{
		fprintf(stderr, "can't push token to tokens\n");
		return NULL;
	}

	if (!jtokset(&dest->toks[dest->size], tok.value, tok.size))
	{
		fprintf(stderr, "cant push token to tokens\n");
		return NULL;
	}

	dest->toks[dest->size].size = tok.size;
	dest->toks[dest->size].type = tok.type;
	return dest->toks + dest->size++;
}
