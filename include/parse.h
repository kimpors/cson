#pragma once
#include "token.h"
#include <stdbool.h>

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JType;

typedef struct jitem JItem;
typedef struct jvalue JValue;

typedef struct jitem {
	char *key;
	JType type;
	union {
		double num;
		char *str;
		bool boo;
		JItem *obj;
		JValue *arr;
	} value;
} JItem;

typedef struct jvalue {
	JType type;
	union {
		double num;
		char *str;
		bool boo;
		JItem *obj;
		JValue *arr;
	} value;
} JValue;

void jprintval(JValue *arr);
void jprintitem(JItem *restrict item);
JItem *jparseitem(JToken *toks, size_t lim);
JValue *jparseval(JToken *toks, size_t lim);

