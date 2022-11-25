#include "util.h"
#include <stdlib.h>

void list_init(list *list, uint size) {
	list->elements = NULL;
	list->element_count = 0;
	list->element_max = 0;
	list->element_size = size;
}


void *list_get_new(list *list) {
	if (list->element_count == list->element_max) {
		void *elements_new;
		if (list->element_max) {
			list->element_max *= 2;
			elements_new = realloc(list->elements, list->element_max * list->element_size);
		} else {
			list->element_max = 1;
			elements_new = malloc(list->element_max * list->element_size);
		}

		if (elements_new == NULL) {
			printf("Failed to resize list to %d elements!\n", list->element_size);
		}
		list->elements = elements_new;
	}

	void *element = list->elements + list->element_count * list->element_size;
	list->element_count++;
	return element;
}


void *list_get(list *list, uint index) {
	if (index >= list->element_count) { return NULL; }
	return list->elements + list->element_size * index;
}
