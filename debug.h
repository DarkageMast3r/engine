#ifndef DEBUG_H
#define DEBUG_H

typedef unsigned int uint;

#include "COMPILE_SETTINGS"

#ifdef DEBUG_MEM



void *debug_malloc(uint bytes, uint line, char *file);
void *debug_realloc(void *ptr, uint bytes, uint line, char *file);
void debug_free(void *ptr, uint line, char *file);

//#warning "Got here"
#define malloc(s) debug_malloc(s, __LINE__, __FILE__)
#define realloc(s, t) debug_realloc(s, t, __LINE__, __FILE__)
#define free(s) debug_free(s, __LINE__, __FILE__)

#endif

#endif
