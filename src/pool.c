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

	switch (type)
	{
		case JTOKEN:
			if(!(tokpool[id].data = calloc(capacity, sizeof(JToken))))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}

			tokpool[id].capacity = capacity;
			toksbuf[id].capacity = capacity;
			toksbuf[id].toks = tokpool[id].data;
			return &tokpool[id].data;
		case JTOKEN_STR:
			if(!(tokvalpool[id].data = calloc(capacity, sizeof(char))))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}

			tokvalpool[id].capacity = capacity;
			return &tokvalpool[id].data;
	}

	return NULL;
}

void *jpoolexpand(size_t id, JPoolType type)
{
	JPool *tok = tokpool + id;
	JPool *val = tokvalpool + id;

	switch (type)
	{
		case JTOKEN:
			if (!(tok->data = realloc(tok->data, tok->capacity * 1.5)))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}

			toksbuf->capacity *= 1.5;
			tok->capacity *= 1.5;
			printf("tok cap: %ld\n", tok->capacity);
			return tok->data;
		case JTOKEN_STR:
			if (!(val->data = realloc(val->data, val->capacity * 2)))
			{
				fprintf(stderr, "cannot allocate memory in pool init =_=");
				return NULL;
			}

			val->capacity *= 2;
			printf("str cap: %ld\n", val->capacity);
			return val->data;
	}

	return NULL;
}

void *jpoolpush(size_t id, JPoolType type, void *data, size_t size)
{
	JTokens *buf = toksbuf + id;
	JPool *tok = tokpool + id;
	JPool *val = tokvalpool + id;
	JToken *temp;

	switch (type)
	{
		case JTOKEN:
			if (tok->size + size >= tok->capacity &&
					!jpoolexpand(id, JTOKEN)) return NULL;

			temp = data;
			memcpy(buf->toks + buf->size, data, size);
			jpoolpush(id, JTOKEN_STR, temp->value, strlen(temp->value));
			buf->size++;
			return buf->toks + buf->size - 1;
		case JTOKEN_STR:
			if (val->size + size >= val->capacity &&
					!jpoolexpand(id, JTOKEN_STR)) return NULL;

			buf->toks[buf->size].value = val->data + val->size;
			memcpy(buf->toks[buf->size].value, data, size);
			buf->toks[buf->size].value[size] = '\0';
			val->size += size + 1;
			return buf->toks[buf->size].value;
	}

	return NULL;
}

JTokens *jgettoks(void)
{
	toksbuf[currtok].id = currtok;
	return toksbuf + currtok++;
}
