#include <string.h>
#include <ctype.h>

#include "token.h"
#include "jarray.h"

void jtokprint(JToken *restrict tok)
{
	switch (tok->type)
	{
		case BRACKET:
			printf("[TYPE: %9s]\t[VALUE: '%s']\n", "bracket", tok->str);
			break;
		case SEPARATOR:
			printf("[TYPE: %9s]\n", "separator");
			break;
		case COMA:
			printf("[TYPE: %9s]\n", "coma");
			break;
		case VALUE:
			printf("[TYPE: %9s]\t[VALUE: '%s']\n", "value", (char *)tok->str);
			break;
	}
}

char *jtokget(JToken *dest, char *s, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;
	if (!*s) return NULL;

	char *ps;
	char *temp = NULL;
	switch (*s)
	{
		case '{': case '}':
		case '[': case ']':
			jinit(temp, 2);
			jlen(temp) = 1;
			temp[0] = s[0];
			temp[1] = '\0';

			dest->str = temp;
			dest->type = BRACKET;
			s++;
			break;
		case ':':
			dest->type = SEPARATOR;
			s++;
			break;
		case ',':
			dest->type = COMA;
			s++;
			break;
		case '"':
			ps = s;
			while (*ps != '\0' && *++ps != '"');

			jinit(temp, (ps - s + 1));
			jlen(temp) = (ps - s);
			strncpy(temp, s, ps - s);
			temp[ps - s] = '\0';

			dest->str = temp;
			dest->type = VALUE;
			s = ps + 1;
			break;
		default:
			ps = s;
			while (isalnum(*++ps) || *ps == '.');
			// while (*++ps != ',' && *ps != ']' && *ps != '}');

			jinit(temp, (ps - s + 1));
			jlen(temp) = (ps - s);
			strncpy(temp, s, ps - s);
			temp[ps - s] = '\0';

			dest->type = VALUE;
			dest->str = temp;
			s = ps;
			break;
	}
	return s;
}

void jtoksprint(JToken *toks)
{
	printf("[SIZE: %4ld][CAPACITY: %4ld]\n", jlen(toks), jcap(toks));
	printf("====================================\n");

	for (size_t i = 0; i < jlen(toks); i++)
	{
		jtokprint(&toks[i]);
	}
}
