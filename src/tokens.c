#include "tokens.h"
#include <stdlib.h>
#include <string.h>

void jtoksprint(JTokens *toks)
{
	printf("[SIZE: %4ld][CAPACITY: %4ld]\n", toks->size, toks->capacity);
	printf("====================================\n");

	for (size_t i = 0; i < toks->size; i++)
	{
		jtokprint(toks->toks + i);
	}
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

JToken *jtoksget(JTokens *dest, char *s, size_t lim)
{
	char *ps = s;
	JToken tok = { 0 };

	while ((ps = jtokget(&tok, ps, lim)))
	{
		jtokspush(dest, tok);
		tok.capacity = 0;
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
