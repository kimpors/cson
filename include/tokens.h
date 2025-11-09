#pragma once
#include "token.h"

typedef struct {
	JToken *toks;
	size_t size;
	size_t capacity;
} JTokens;

void jtoksprint(JTokens *toks);
JToken *jtokspush(JTokens *dest, JToken tok);
JToken *jtoksinit(JTokens *dest, size_t capacity);
JToken *jtoksexpand(JTokens *dest, size_t capacity);
JToken *jtoksget(JTokens *dest, char *s, size_t lim);
