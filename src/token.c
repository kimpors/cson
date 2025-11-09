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

char *jtokget(JToken *dest, char *s, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;
	if (!*s) return NULL;

	char *ps;
	switch (*s)
	{
		case '{': case '}':
		case '[': case ']':
			dest->size = 1;
			dest->type = BRACKET;
			jtokset(dest, s, dest->size);
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
			dest->size = ps - s;
			jtokset(dest, s, dest->size);
			s = ps + 1;
			break;
		default:
			ps = s;
			while (isalnum(*++ps));
			dest->size = ps - s;
			dest->type = VALUE;
			jtokset(dest, s, dest->size);
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
		jtokinit(dest, size);
		dest->size = size;
	}
	if (size >= dest->capacity &&
			!jtokexpand(dest, (dest->capacity + size) * 2))
	{
		fprintf(stderr, "can't set token\n");
		return NULL;
	}

	strncpy(dest->value, s, size);
	dest->value[size] = '\0';
	return dest->value;
}
