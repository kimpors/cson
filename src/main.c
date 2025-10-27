#include "parse.h"
#include "token.h"
#include <stdlib.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1024

static FILE *fp;
static char buf[MAX_FILE_SIZE] = "{\n\t\"items\":[\"apple\", null, false, true, 240, \"bananan\"]\n}";

size_t jindex = 0;
JToken toks[255];

int main(int argc, char *argv[])
{
	JTokens *arr = jtoksinit(20);

	printf("id: %ld\n", arr->id);
	printf("cap: %ld\n", arr->capacity);

	jtokspush(arr, VALUE, "HELLO");
	jtokspush(arr, VALUE, "THERE");
	printf("val: %s\n", arr->toks[0].value);
	printf("val: %s\n", arr->toks[1].value);

	return 0;

	size_t len;
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

	JToken tok;
	char *ps = buf;
	while ((ps = jgettoken(ps, &toks[jindex++], MAX_FILE_SIZE)))
	{
		jprinttok(&toks[jindex - 1]);
	}

	puts("");

	JObject *buf = jparse(toks, jindex - 1, true);
	jprintobj(buf, true);

	return 0;
}
