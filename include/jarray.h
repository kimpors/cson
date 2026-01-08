#pragma once
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

#define jinit(arr, cap) {if (cap) { \
							if ((arr = malloc(sizeof(Header) + sizeof(*arr) * cap))) { \
								arr = (void *)((char *)arr + sizeof(Header)); \
								jlen(arr) = 0; \
								jcap(arr) = cap; \
							} else {\
									fprintf(stderr, "error: Cant init array in 'jinit'\n"); \
							} \
						}}

#define jexpand(arr, cap) { if (arr && cap && cap > jcap(arr)) { \
								if ((arr = realloc(jheader(arr), sizeof(Header) + sizeof(*arr) * cap))) { \
									arr = (void *)((char *)arr + sizeof(Header)); \
									jcap(arr) = cap; \
								} else { \
									fprintf(stderr, "error: Can't expand array in 'jexpand'\n"); \
								} \
							}}

#define jpush(arr, val)	if (arr) { \
							if (jlen(arr) + 1 < jcap(arr)) { \
								arr[jlen(arr)++] = val; \
							} else { \
								jexpand(arr, jcap(arr) * 2); \
								arr[jlen(arr)++] = val; \
							}\
						} else {\
							jinit(arr, 2); \
							jlen(arr) = 0; \
							arr[jlen(arr)++] = val; \
						} 

#define jstrncpy(arr, str, lim) \
						if (arr) { \
							size_t len = strlen(str); \
							len = len > lim ? lim : len; \
							if (len < jcap(arr)) { \
								jlen(arr) = len; \
								strncpy(arr, str, jlen(arr)); \
								arr[jlen(arr)] = '\0'; \
							} else { \
								jexpand(arr, (jcap(arr) + (len * 2))); \
								jlen(arr) = len; \
								strncpy(arr, str, jlen(arr)); \
								arr[jlen(arr)] = '\0'; \
							} \
						} else { \
							size_t len = strlen(str); \
							len = len > lim ? lim : len; \
							jinit(arr, (len * 2)); \
							jlen(arr) = len; \
							strncpy(arr, str, jlen(arr)); \
							arr[jlen(arr)] = '\0'; \
						}

#define jconcat(arr, str)  if (arr) { \
							size_t len = strlen(str); \
							if (jlen(arr) + len < jcap(arr)) { \
								strcat(arr, str); \
								jlen(arr) += len; \
								arr[jlen(arr)] = '\0'; \
							} else {\
								jexpand(arr, ((jcap(arr) + len) * 2)); \
								jlen(arr) += len; \
								strcat(arr, str); \
								arr[jlen(arr)] = '\0'; \
							} \
						} else { \
							size_t len = strlen(str); \
							jinit(arr, (len * 2)); \
							jlen(arr) = len; \
							strcat(arr, str); \
							arr[jlen(arr)] = '\0'; \
						}
