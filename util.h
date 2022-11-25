#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>

typedef unsigned int uint;

typedef struct {
	void *elements;
	uint element_count;
	uint element_max;
	uint element_size;
} list;

#define LIST_INIT(t) {.elements = NULL, .element_count = 0, .element_max = 0, .element_size = sizeof(t)}

void list_init(list *list, uint size);
void *list_get_new(list *list);
void *list_get(list *list, uint index);

#endif
