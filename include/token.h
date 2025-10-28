#pragma once
#include <stdio.h>

typedef enum {
	VALUE, BRACKET,
	COMA, SEPARATOR
} JTokenType;

typedef struct {
	char *value;
	JTokenType type;
} JToken;

typedef struct {
	JToken *toks;
	size_t id;
	size_t size;
	size_t capacity;
} JTokens;

void jtoksprint(JTokens *toks);
void jtokprint(JToken *restrict tok);
char *jgettoken(char *s, JToken *restrict dest, size_t lim);

JTokens *jtoksinit(size_t capacity);
JToken *jtokspush(JTokens *dest, JTokenType type, char *val);
