#pragma once
#include <stdio.h>

typedef enum {
	VALUE, BRACKET,
	COMA, SEPARATOR
} JTokenType;

typedef struct {
	char *value;
	JTokenType type;
	size_t size;
	size_t capacity;
} JToken;

typedef struct {
	JToken *toks;
	size_t size;
	size_t capacity;
} JTokens;

void jtoksprint(JTokens *toks);
void jtokprint(JToken *restrict tok);
char *jgettoken(char *s, JTokens *dest, size_t lim);

char *jtokinit(JToken *dest, size_t capacity);
char *jtokset(JToken *dest, const char *s, size_t size);

JToken *jtokspush(JTokens *dest, JToken tok);
JToken *jtoksinit(JTokens *dest, size_t capacity);
