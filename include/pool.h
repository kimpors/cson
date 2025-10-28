#include "token.h"

#define BUF_SIZE 32
#define INIT_ALLOC 128

typedef enum {
	JTOKEN, JTOKEN_STR,
} JPoolType;

typedef struct {
	void *data;
	size_t size;
	size_t capacity;
} JPool;

JTokens *jgettoks(void);
void *jpoolinit(size_t id, JPoolType type, size_t capacity);
void *jpoolpush(size_t id, JPoolType type, void *data, size_t size);
