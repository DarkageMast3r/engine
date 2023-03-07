#ifndef IMAGE_H
#define IMAGE_H

typedef unsigned int uint;

typedef struct {
	unsigned char *pixels;
	int width, height;
	int channel_count;
	uint texture_name;
} image;

image *image_load(char *path);


#endif
