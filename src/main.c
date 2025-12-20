#include <stdio.h>
#include <unistd.h>
#include "jarray.h"
#include "parse.h"
#include "token.h"

#define MAX_FILE_SIZE 1024

static FILE *fp;
static char buf[MAX_FILE_SIZE] = "{\n\t\"name\":[\"apple\", 34.4, \"hello\"], \"tmp\": { \"Hello\":23 }, \"isman\": false\n}";

int main(void)
{
	char *ps = buf;
	JToken tok = { 0 };
	JToken *toks = NULL;

	while ((ps = jtokget(&tok, ps, MAX_FILE_SIZE)))
	{
		jpush(toks, tok);
	}

	// jtoksprint(toks);

	JItem *items = NULL;
	items = jparse(toks);
	jprint(items);

	jfree(items);
	jfree(toks);
	return 0;
}
