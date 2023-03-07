#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

char *file_read(char *path) {
        FILE *file = fopen(path, "r");
        if (file == NULL) {
                printf("Failed to open file '%s'\n", path);
                return NULL;
        }

        fseek(file, 0, SEEK_END);

        long int length = ftell(file);
        fseek(file, 0, SEEK_SET);

	printf("Opening new file %ld chars long\n", length);
        char *source = malloc((length) * sizeof(char));
        fread(source, length * sizeof(char), 1, file);
        source[length] = 0;

        return source;

}

