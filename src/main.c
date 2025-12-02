#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "jarray.h"
#include "parse.h"
#include "token.h"

#define MAX_FILE_SIZE 1024

static FILE *fp;
// static char buf[MAX_FILE_SIZE] = "{\n\t\"items\":[\"apple\", null, false, true, 240, \"bananan\"]\n}";
static char buf[MAX_FILE_SIZE] = "{\n\t\"name\":\"apple\", \"age\": 34, \"isman\": false\n}";

int main(int argc, char *argv[])
{
	size_t len = 0;
	if (argc >= 2 && (fp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "cson(error): can't open file %s\n", argv[1]);
		exit(1);
	}

	if (argc >= 2 && !(len = fread(buf, 1, MAX_FILE_SIZE, fp)))
	{
		fprintf(stderr, "cson(error): can't read file %s\n", argv[1]);
		exit(2);
	}

	if (len) buf[len] = '\0';

	char *ps = buf;
	JToken tok = { 0 };
	JToken *toks = NULL;

	while ((ps = jtokget(&tok, ps, MAX_FILE_SIZE)))
	{
		jpush(toks, tok);
	}

	jtoksprint(toks);
	jfree(toks);
	return 0;
}
