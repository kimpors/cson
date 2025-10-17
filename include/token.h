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

void print_tok(JToken *tok);
char *jgettoken(char *s, JToken *restrict dest, size_t lim);
