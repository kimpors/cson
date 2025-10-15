#include "token.h"
#include <stdlib.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1024

static FILE *fp;
static char buf[MAX_FILE_SIZE] = "{\"name\":\"John\", \"age\":12}";

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
	while ((ps = gettoken(ps, &tok, MAX_FILE_SIZE)))
	{
		print_tok(&tok);
	}

	return 0;
}
