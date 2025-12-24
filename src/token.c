#include <string.h>
#include <ctype.h>

#include "jerror.h"
#include "jtoken.h"
#include "jarray.h"

void jtokprint(JToken *restrict tok)
{
	if (!tok) 
	{
		JERROR_MSG("'tok' is unintilized");
		return;
	}

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
		default:
			JWARN_MSG("'tok' type is not specified (type: %u, value: %s)", tok->type, tok->str);
			break;
	}
}

char *jtokenize(JToken *dest, char *s, size_t lim)
{
	if (!s || !*s) return NULL;
	while (lim-- > 0 && isspace(*s)) s++;
	if (!*s) return NULL;

	char *ps;
	switch (*s)
	{
		case '{': case '}':
		case '[': case ']':
			dest->str = NULL;
			jstrncpy(dest->str, s, 1);
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

			dest->str = NULL;
			jstrncpy(dest->str, s, (ps - s));
			dest->type = VALUE;
			s = ps + 1;
			break;
		default:
			ps = s;
			while (isalnum(*++ps) || *ps == '.');

			dest->str = NULL;
			jstrncpy(dest->str, s, (ps - s));
			dest->type = VALUE;
			s = ps;
			break;
	}
	return s;
}

JToken *jtokenizeall(char *s, size_t lim)
{
	if (!s || !*s) return NULL;

	JToken tmp = { 0 };
	JToken *toks = NULL;

	while ((s = jtokenize(&tmp, s, lim)))
	{
		jpush(toks, tmp);
	}

	return toks;
}

void jtokprintall(JToken *toks)
{
	if (!toks) 
	{
		JERROR_MSG("'toks' is uninitilized");
		return;
	}

	printf("[SIZE: %4ld][CAPACITY: %4ld]\n", jlen(toks), jcap(toks));
	printf("====================================\n");

	for (size_t i = 0; i < jlen(toks); i++)
	{
		jtokprint(&toks[i]);
	}
}
