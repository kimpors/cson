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

typedef struct {
	JValue *values;
	size_t size;
	char **keys;
} JObject;

typedef struct jvalue {
	JType type;
	union {
		double num;
		char *str;
		bool boo;
		JObject *obj;
	} value;
} JValue;

JObject *jparse(JToken *toks, size_t lim, bool isobj);
void jprintobj(JObject *obj, bool isobj);
