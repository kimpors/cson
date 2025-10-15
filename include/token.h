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
char *gettoken(char *s, JToken *dest, size_t lim);
