#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum {
	// types
	BRACKET, NAME,

	// value types
	NONE, NUMBER, STRING,
	BOOL, ARRAY, OBJECT,
	NIL
} TokenType;

// for array type
typedef struct {
	TokenType type;
	void *value;
} Item;

typedef struct {
	void *value;
	TokenType type;
} Token;

void print_tok(Token *tok);
char *gettoken(char *s, Token *dest, size_t lim);
