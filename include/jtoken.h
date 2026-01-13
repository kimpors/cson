#pragma once
#include <stdlib.h>
#include <stdbool.h>

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
char *jtoktojson(JToken *toks, bool strip);
