#include <stdlib.h>
#include <stdio.h>
typedef unsigned int uint;

void *debug_malloc(uint bytes, uint line, char *file) {
	printf("Allocating %d bytes @ %s > %d\n\n", bytes, file, line);
	fflush(stdout);
	return malloc(bytes);
}

void *debug_realloc(void *ptr, uint bytes, uint line, char *file) {
	printf("Reallocating to %d bytes in file %s, line %d\n\n", bytes, file, line);
	fflush(stdout);
	return realloc(ptr, bytes);
}

void debug_free(void *ptr, uint line, char *file) {
	printf("Freeing pointer @ %s > %d\n\n", file, line);
	fflush(stdout);
	free(ptr);
}
