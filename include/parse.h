#pragma once
#include "token.h"

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JItemType;

typedef struct {
	char *key;
	void *value;
	JItemType type;
} JItem;

void print_item(JItem *restrict item);
JItem *parse(JToken *toks, size_t lim);
