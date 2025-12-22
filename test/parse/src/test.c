#include <stdio.h>
#include "test.h"
#include "jarray.h"
#include "jparse.h"

void jprint_test(JItem *items)
{
	for (size_t i = 0; i < jlen(items); i++)
	{
		switch (items[i].type)
		{
			case NIL:
				if (items[i].key) printf("%s - null\n", items[i].key);
				else printf("null, ");
				break;
			case STRING:
				if (items[i].key) printf("%s - \"%s\"\n", items[i].key, items[i].value.str);
				else printf("\"%s\", ",items[i].value.str);
				break;
			case NUMBER:
				if (items[i].key) printf("%s - %.2f\n", items[i].key, items[i].value.num);
				else printf("%.2f, ", items[i].value.num);
				break;
			case BOOL:
				if (items[i].key) printf("%s - %s", items[i].key, items[i].value.boo ? "true" : "false");
				else printf("%s, ", items[i].value.boo ? "true" : "false");
				break;
			case ARRAY:
				printf("%s - [", items[i].key);
				jprint_test(items[i].value.arr);
				puts("]");
				break;
			case OBJECT:
				printf("%s - {", items[i].key);
				jprint_test(items[i].value.arr);
				puts("}");
			  break;
			}
        }
}
