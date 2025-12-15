#pragma once
#include "token.h"
#include <stdbool.h>

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JType;

typedef struct jvalue JValue;
typedef struct jobject JObject;

typedef struct jvalue {
	JType type;
	union {
		double num;
		bool boo;
		char *str;
		JValue *arr;
		JObject *obj;
	} value;
} JValue;

typedef struct jobject {
	char *key;
	JValue val;
} JObject;

void jprintobj(JObject *obj);
JObject *jparse(JToken *toks);
