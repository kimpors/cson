#pragma once
#include "token.h"
#include <stdbool.h>

typedef enum {
	NIL, STRING, 
	NUMBER, BOOL,
	ARRAY, OBJECT
} JType;

typedef struct jarray JArray;
typedef struct jobject JObject;

typedef struct jvalue {
	JType type;
	union {
		double num;
		bool boo;
		char *str;
		JArray *arr;
		JObject *obj;
	} value;
} JValue;

typedef struct jobject {
	char *key;
	JValue val;
} JObject;

typedef struct jarray {
	char *key;
	JValue *vals;
} JArray;

void jprintobj(JObject *obj, bool isobj);
JObject *jparse(JToken *toks, size_t lim, bool isobj);
