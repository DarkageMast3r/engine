#include "image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>






#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"




image textures[32];
uint texture_count = 0;

image *image_load(char *path) {
	stbi_set_flip_vertically_on_load(1);
	image *img = &textures[texture_count];
	texture_count++;

	unsigned char *pixels = stbi_load(path, &img->width, &img->height, &img->channel_count, 0);
	img->pixels = pixels;

	glGenTextures(1, &img->texture_name);


	glBindTexture(GL_TEXTURE_2D, img->texture_name);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA8,
		img->width,
		img->height,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		pixels
	);




	return img;
}
