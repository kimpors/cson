#pragma once
#include "token.h"
#include <stdbool.h>

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JType;

typedef struct jitem JItem;

typedef struct jitem {
	char *key;
	JType type;
	union {
		double num;
		bool boo;
		char *str;
		JItem *arr;
	} value;
} JItem;

void jprint(JItem *items);
JItem *jparse(JToken *toks);
