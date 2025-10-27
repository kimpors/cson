#include "pool.h"
#include <string.h>
#include "token.h"
#include <stdlib.h>
#include <assert.h>

#define BUF_SIZE 32
#define POOL_INIT_SIZE 255

static size_t currtok = 0; 
static JTokens toksbuf[BUF_SIZE];
static JPool tokpool[BUF_SIZE];
static JPool tokvalpool[BUF_SIZE];


void *jpoolinit(size_t id, JPoolType type, size_t capacity)
{
	if (!capacity) capacity = POOL_INIT_SIZE; 

	JPool *pool;
	switch (type)
	{
		case TOKEN:
			if(!(tokpool[id].data = calloc(capacity, sizeof(JToken))))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}


			if(!(tokvalpool[id].data = calloc(capacity, sizeof(char))))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}

			tokpool[id].capacity = capacity;
			tokvalpool[id].capacity = capacity;
			toksbuf[id].capacity = capacity;
			toksbuf[id].toks = tokpool[id].data;
			return &tokpool[id];
	}

	return NULL;
}


void *jpoolpush(size_t id, JPoolType type, void *data, size_t size)
{
	JTokens *buf = toksbuf + id;
	JPool *tok = tokpool + id;
	JPool *val = tokvalpool + id;
	JToken *temp;

	void *res;

	switch (type)
	{
		case TOKEN:
			printf("buf size: %ld\n", buf->size);
			temp = data;
			size_t len = strlen(temp->value);
			memcpy(buf->toks + buf->size, data, size);
			buf->toks[buf->size].value = val->data + val->size;
			memcpy(buf->toks[buf->size].value, temp->value, len);
			buf->toks[buf->size].value[len] = '\0';
			val->size += len + 1;
			buf->size++;
			return buf->toks + buf->size - 1;
	}

	return res;
}

JTokens *jgettoks(void)
{
	toksbuf[currtok].id = currtok;
	return toksbuf + currtok++;
}
