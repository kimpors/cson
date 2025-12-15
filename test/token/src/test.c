#include <stdio.h>
#include "test.h"
#include "jarray.h"

void jtokprint_test(JToken *restrict tok)
{
	switch (tok->type)
	{
		case BRACKET:
			printf("bracket - %s\n", tok->str);
			break;
		case SEPARATOR:
			printf("separator\n");
			break;
		case COMA:
			printf("coma\n");
			break;
		case VALUE:
			printf("value - %s\n", tok->str);
			break;
	}
}

void jtoksprint_test(JToken *toks)
{
	printf("%ld, %ld\n", jlen(toks), jcap(toks));

	for (size_t i = 0; i < jlen(toks); i++)
	{
		jtokprint_test(&toks[i]);
	}
}
