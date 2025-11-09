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

void jtokprint(JToken *restrict tok);
char *jtokinit(JToken *dest, size_t capacity);
char *jtokget(JToken *dest, char *s, size_t lim);
char *jtokset(JToken *dest, const char *s, size_t size);
