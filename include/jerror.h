#pragma once
#define JERROR_MSG(fmt, ...) (fprintf(stderr, "CSON [ERROR]:" fmt " (file: %s, line: %d)\n", ##__VA_ARGS__, __FILE__, __LINE__));
#define JWARN_MSG(fmt, ...) (fprintf(stderr, "CSON [WARNING]:" fmt " (file: %s, line: %d)\n", ##__VA_ARGS__, __FILE__, __LINE__));
