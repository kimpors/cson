#pragma once
#include "token.h"
#include <stdbool.h>

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JItemType;

typedef struct jitem JItem;
typedef struct jvalue JValue;

typedef struct jitem {
	char *key;
	JItemType type;
	union {
		double num;
		char *str;
		bool boo;
		JItem *obj;
		JValue *arr;
	} value;
} JItem;

typedef struct jvalue {
	JItemType type;
	union {
		double num;
		char *str;
		bool boo;
		JItem *obj;
		JValue *arr;
	} value;
} JValue;

void print_item(JItem *restrict item);
JItem *parse(JToken *toks, size_t lim);
