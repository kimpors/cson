#include <stdio.h>
#include "test.h"
#include "jarray.h"

void jtokprint_test(JToken *restrict tok)
{
	switch (tok->type)
	{
		case BRACKET:
			printf("%s", tok->str);
			break;
		case SEPARATOR:
			printf(":");
			break;
		case COMA:
			puts(",");
			break;
		case VALUE:
			printf("%s", tok->str);
			break;
	}
}
