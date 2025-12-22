#include <stdio.h>
#include <unistd.h>
#include "jarray.h"
#include "test.h"
#include "jtoken.h"
#include "jparse.h"

#define CSON_MAX_BUF	1024

static FILE *fp;
static char buf[CSON_MAX_BUF];

int main(int argc, char *argv[])
{
	if (argc != 2) 
	{
		fprintf(stderr, "error: :)");
		return 1;
	}

	if (!(fp = fopen(argv[1], "r")))
	{
		fprintf(stderr, "file e r r o r: :(");
		return 2;
	}

	int ch;
	size_t i = 0;

	while ((ch = fgetc(fp)) != EOF)
	{
		buf[i++] = ch;
	}

	fclose(fp);

	char *ps = buf;
	JToken tok = { 0 };
	JToken *toks = NULL;

	toks = jtokenizeall(ps, CSON_MAX_BUF);

	// while ((ps = jtokget(&tok, ps, CSON_MAX_BUF)))
	// {
	// 	jpush(toks, tok);
	// }

	JItem *items = NULL;
	items = jitemparse(toks);

	jprint_test(items);

	jfree(items);
	jfree(toks);
	return 0;
}
