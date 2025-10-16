#include "parse.h"
#include "token.h"
#include <stdlib.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1024

static FILE *fp;
static char buf[MAX_FILE_SIZE] = "{\"name\":\"John\", \"age\":12 }";

size_t jindex = 0;
JToken toks[255];

int main(int argc, char *argv[])
{
	if (argc >= 2 && (fp = fopen(argv[1], "r")) == NULL)
	{
		fprintf(stderr, "cson(error): can't open file %s\n", argv[1]);
		exit(1);
	}

	if (argc >= 2 && !fread(buf, 1, MAX_FILE_SIZE, fp))
	{
		fprintf(stderr, "cson(error): can't read file %s\n", argv[1]);
		exit(2);
	}

	JToken tok;
	char *ps = buf;
	while ((ps = gettoken(ps, &toks[jindex++], MAX_FILE_SIZE)));
	JItem *items = parse(toks, jindex - 1);

	for (size_t i = 0; i < 2; i++)
	{
		print_item(&items[i]);
	}

	return 0;
}
