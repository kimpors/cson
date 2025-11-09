#include "token.h"
#include <stdlib.h>
#include <unistd.h>

#define MAX_FILE_SIZE 1024

static FILE *fp;
// static char buf[MAX_FILE_SIZE] = "{\n\t\"items\":[\"apple\", null, false, true, 240, \"bananan\"]\n}";
static char buf[MAX_FILE_SIZE] = "{\n\t\"name\":\"apple\", \"age\": 34, \"isman\": false\n}";

size_t jindex = 0;
JToken toks[255];

int main(int argc, char *argv[])
{
	JTokens arr = { 0 };
	jtoksinit(&arr, 2);

	// for (size_t i = 0; i < arr.capacity; i++)
	// {
	// 	jtokinit(&arr.toks[i], 32);
	// }

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

	JToken tok;
	char *ps = buf;
	while ((ps = jgettoken(ps, &arr, MAX_FILE_SIZE)));
	jtoksprint(&arr);

	exit(1);
	return 0;
}
