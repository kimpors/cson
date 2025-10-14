#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef enum {
	NONE, BRACKET, COMA,
	SEPARATOR, NUMBER,
	STRING, BOOL, NIL
} TokenType;

typedef struct {
	void *value;
	TokenType type;
} Token;

void print_tok(Token *tok);
char *gettoken(char *s, Token *dest, size_t lim);
