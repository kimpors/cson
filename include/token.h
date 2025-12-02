#pragma once
#include "stdlib.h"

typedef enum {
	VALUE, BRACKET,
	COMA, SEPARATOR
} JTokenType;

typedef struct {
	char *str;
	JTokenType type;
} JToken;

void jtoksprint(JToken *toks);
void jtokprint(JToken *restrict tok);
char *jtokget(JToken *dest, char *s, size_t lim);
