#pragma once
#include "token.h"

#define JNIL(A) (long)(A)
#define JBOO(A) (bool)(A)
#define JSTR(A) (char *)(A)
#define JNUM(A)	*((double *)(A))  

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
