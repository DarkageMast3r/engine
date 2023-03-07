#include "util.h"
#include "world.h"
#include <glad/glad.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug.h"

shader shader_list[16] = {};
uint shader_count = 0;


uint shader_compile(char *source, uint type) {
	uint shader_id = glCreateShader(type);
	glShaderSource(shader_id, 1, (const char* const*) &source, NULL);
	glCompileShader(shader_id);

	int result;
	glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);

		char *message;
		message = malloc(sizeof(*message) * (length+1));
		glGetShaderInfoLog(shader_id, length, &length, message);
		printf(
			"%s shader compilation error. Source:\n"
			"%s\n\n"
			"Message:\n"
			"%s\n========\n",
			type == GL_VERTEX_SHADER ? "Vertex" : "Fragment",
			source,
			message
		);
	}

	return shader_id;
}

shader *shader_create_vf(char *vertex_source, char *fragment_source) {
	uint program = glCreateProgram();

	uint shader_vertex = shader_compile(vertex_source, GL_VERTEX_SHADER);
	uint shader_fragment = shader_compile(fragment_source, GL_FRAGMENT_SHADER);

	glAttachShader(program, shader_vertex);
	glAttachShader(program, shader_fragment);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(shader_vertex);
	glDeleteShader(shader_fragment);

	shader *shader = shader_list + shader_count;
	shader_count++;
	printf("Created new shader at %lx\n", (long int) shader);


	shader->shader_id = program;
	list_init(&shader->uniforms, sizeof(uniform));


	return shader;
}

int shader_get_uniform_location(shader *shader, char *uniform_name) {
	glUseProgram(shader->shader_id);
	uint i;
	for (i = 0; i < shader->uniforms.element_count; i++) {
		uniform *uniform = list_get(&shader->uniforms, i);
		if (strcmp(uniform_name, uniform->name) == 0) {
			return uniform->location;
		}

	}

	int location = glGetUniformLocation(shader->shader_id, uniform_name);	
	uniform *uniform = list_get_new(&shader->uniforms);
	uniform->location = location;

	uint name_len = strlen(uniform_name) + 1;
	uniform->name = malloc(sizeof(*uniform->name) * name_len);
	strcpy(uniform->name, uniform_name);

	return location;
}
