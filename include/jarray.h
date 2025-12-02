#include <stdio.h>
#include <stdlib.h>

typedef struct {
	size_t len;
	size_t cap;
} Header;

#define jheader(a) 	((Header *)a - 1)
#define jlen(a)		(jheader(a)->len) 
#define jcap(a)		(jheader(a)->cap) 
#define jfree(arr)	free(jheader(arr))

#define jinit(arr, cap) if (cap) { \
							if ((arr = malloc(sizeof(Header) + sizeof(*arr) * cap))) { \
								arr = (void *)((char *)arr + sizeof(Header)); \
								jlen(arr) = 0; \
								jcap(arr) = cap; \
							} else {\
									fprintf(stderr, "error: Cant init array in 'jinit'\n"); \
							} \
						}

#define jexpand(arr, cap)	if (arr && cap && cap > jcap(arr)) { \
								if ((arr = realloc(jheader(arr), sizeof(Header) + sizeof(*arr) * cap))) { \
									arr = (void *)((char *)arr + sizeof(Header)); \
									jcap(arr) = cap; \
								} else { \
									fprintf(stderr, "error: Can't expand array in 'jexpand'\n"); \
								} \
							}


#define jpush(arr, val)	if (arr) { \
							if (jlen(arr) + 1 < jcap(arr)) { \
								arr[jlen(arr)++] = val; \
							} else if ((arr = realloc(jheader(arr), sizeof(Header) + sizeof(*arr) * jcap(arr) * 2))) { \
								arr = (void *)((char *)arr + sizeof(Header)); \
								jcap(arr) = jcap(arr) * 2; \
								arr[jlen(arr)++] = val; \
							} else { \
								fprintf(stderr, "error: Can't expan array in 'jpush'\n"); \
							}\
						} else {\
							if ((arr = malloc(sizeof(Header) + sizeof(*arr) * 2))) { \
								arr = (void *)((char *)arr + sizeof(Header)); \
								jcap(arr) = 2; \
								jlen(arr) = 0; \
								arr[jlen(arr)++] = val; \
							} else {\
								fprintf(stderr, "error: Can't init array in 'jpush'\n"); \
							} \
						} 
