#pragma once
#include "stdlib.h"

typedef enum {
	VALUE, BRACKET,
	COMA, SEPARATOR
} JTokenType;

typedef struct jtok {
	char *str;
	JTokenType type;
} JToken;

void jtokprint(JToken *restrict tok);
void jtokprintall(JToken *toks);

char *jtokenize(JToken *dest, char *s, size_t lim);
JToken *jtokenizeall(char *s, size_t lim);
