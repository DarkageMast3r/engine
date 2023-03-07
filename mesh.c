#include "stdio.h"
#include "vecmath.h"
#include "world.h"
#include "util.h"
#include "file.h"
#include "stddef.h"
#include <stdlib.h>
#include <string.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>


typedef struct {
	int position;
	int uv;
	int normal;
} obj_face;


list meshes[M_RENDER_TYPE_COUNT] = {};

void *min_ptr(void *a, void *b) {
	if ((uint64_t) a < (uint64_t) b) {
		return a;
	}
	return b;
}

void mesh_init() {
	uint i;
	for (i = 0; i < M_RENDER_TYPE_COUNT; i++) {
		list_init(&meshes[i], sizeof(mesh));
	}
}

mesh *mesh_create(vertex *vertices, uint vertex_count, uint *indices, uint index_count, mesh_render_type type) {
	printf("Creating mesh with %d vertices, %d indices\n", vertex_count, index_count);
	mesh *mesh = list_get_new(&meshes[type]);

	glGenBuffers(2, &mesh->buffer_index);
	glGenVertexArrays(1, &mesh->vertex_array);

	mesh->vertices = vertices;
	mesh->indices = indices;
	mesh->vertex_count = vertex_count;
	mesh->index_count = index_count;

	glBindVertexArray(mesh->vertex_array);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_vertex);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_index);
	
	glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(*vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(*indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) offsetof(vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) offsetof(vertex, uv));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (const void *) offsetof(vertex, normal));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	return mesh;
}


typedef enum {
	TO_POSITION,
	TO_NORMAL,
	TO_UV,
	TO_FACE,
	TO_NAME,
	TO_SCALE,
	TO_MAT,
	TO_COUNT
} token_obj;


char *str_findc(char *str, char c) {
	while (*str && (*str != c)) {str++;}
	return str;
}

float str_to_float(char *cur) {
	float sign = 1.0f;
	int dividing = 0;
	float division = 10.0f;
	float value = 0.0f;


	if (*cur == '-') {sign = -1.0f; cur++;}
	while (*cur) {
		if (dividing) {
			division *= 10.0f;
		}
		if (*cur == '.') {
			dividing = 1;
		} else {
			value += (float) (*cur - '0');
			value *= 10.0f;
		}
		cur++;
	}

	return value / division * sign;
}

int str_to_int(char *cur) {
	int val = 0;
	while (*cur) {
		val *= 10;
		val += *cur - '0';
		cur++;
	}

	return val;
}


typedef struct {
	char *name;
	char *image;
} mtl_material;

typedef struct {
	mtl_material *materials;
	uint material_count;
} file_mtl_data;

void mesh_create_from_file_obj(char *path) {
	char *src = file_read(path);

	list positions, normals, faces, uvs;
       	list_init(&positions, sizeof(vec3f));
	list_init(&normals, sizeof(vec3f));
	list_init(&faces, sizeof(obj_face));
	list_init(&uvs, sizeof(vec2f));

	char *p = src;
	while (*p != '\n') {p++;} p++;
	while (*p != '\n') {p++;} p++;	

	token_obj type;
	while (*p) {
		switch (*p) {
			case 'v':
				switch(*(p+1)) {
					case ' ':
						type = TO_POSITION;
						break;
					case 'n':
						type = TO_NORMAL;
						break;
					case 't':
						type = TO_UV;
						break;
				}
				break;
			case 'f':
				type = TO_FACE;
				break;
			case 'o':
				type = TO_NAME;
				break;
			case 's':
				type = TO_SCALE;
				break;
			case 'm':
				if (strncmp(p, "mtllib ", 7) == 0) {
					type = TO_MAT;
					break;
				}
			default:
				type = TO_COUNT;
				break;
		}

		float a, b, c;
		char *start, *end;

		switch(type) {
			case TO_POSITION:

				start = str_findc(p, ' ') + 1;
				end = str_findc(start, ' ');
				*end = 0;
				a = str_to_float(start);
				
				start = end + 1;
				end = str_findc(start, ' ');
				*end = 0;
				b = str_to_float(start);

				start = end + 1;
				end = str_findc(start, ' ');
				*end = 0;
				c = str_to_float(start);

				vec3f *pos = list_get_new(&positions);
				pos->x = a;
				pos->y = b;
				pos->z = c;

				break;


			case TO_NORMAL:

				start = str_findc(p, ' ') + 1;
				end = str_findc(start, ' ');
				*end = 0;
				a = str_to_float(start);

				start = end + 1;
				end = str_findc(start, ' ');
				*end = 0;
				b = str_to_float(start);

				start = end + 1;
				end = str_findc(start, ' ');
				*end = 0;
				c = str_to_float(start);

				
				vec3f *normal = list_get_new(&normals);
				normal->x = a;
				normal->y = b;
				normal->z = c;
				break;

			case TO_UV:

				start = str_findc(p, ' ') + 1;
				end = str_findc(start, ' ');
				*end = 0;
				a = str_to_float(start);

				start = end + 1;
				end = str_findc(start, ' ');
				*end = 0;
				b = str_to_float(start);	

				
				vec2f *uv = list_get_new(&uvs);
				uv->x = a;
				uv->y = b;
				break;
			case TO_FACE:
				start = str_findc(p, ' ') + 1;

				int i;
				for (i = 0; i < 3; i++) {

					end = str_findc(start, '/');		
					*end = 0;


					obj_face *face = (obj_face*) list_get_new(&faces);
					face->position = str_to_int(start);

					start = end + 1;
					end = str_findc(start, '/');
					*end = 0;

					face->uv = str_to_int(start);
					
					start = end + 1;
					end = min_ptr(str_findc(start, ' '), str_findc(start, '\n'));
					char original = *end;
					*end = 0;

					face->normal = str_to_int(start);
					*end = original;


					
					start = end + 1;
				}

				break;
			case TO_MAT:
				start = str_findc(p, ' ') + 1;
				end = str_findc(start, '\n');
				char original = *end;
				*end = 0;
				printf("Material: (%s)", start);
				*end = original;
				break;
			case TO_NAME:
			case TO_SCALE:
				break;
			default:
				break;
		}



//		printf("Cur (%s)\n", p);
		
//		p = str_findc(p, '\n');
		while (*p != '\n') {p++;}
//		printf("Naxt (%s)\n", p);
		p++;
	}

	/*
	printf("Positions:\n");
	uint i;
	for (i = 0; i < positions.element_count; i++) {
		vec3f *pos = list_get(&positions, i);
		printf("%f, %f, %f\n", pos->x, pos->y, pos->z);
	}

	printf("Normals:\n");
	for (i = 0; i < normals.element_count; i++) {
		vec3f *pos = list_get(&normals, i);
		printf("%f, %f, %f\n", pos->x, pos->y, pos->z);
	}
	
	printf("UVs:\n");
	for (i = 0; i < uvs.element_count; i++) {
		vec2f *pos = list_get(&uvs, i);
		printf("%f, %f\n", pos->x, pos->y);
	}

	printf("Faces:\n");
	for (i = 0; i < faces.element_count; i++) {
		obj_face *pos = list_get(&faces, i);
		printf("%d, %d, %d\n", pos->position, pos->normal, pos->uv);
	}
	*/
	uint i;

	vertex *vertices = malloc(sizeof(vertex) * positions.element_count);
	for (i = 0; i < positions.element_count; i++) {
		vec3f *pos = (vec3f *) list_get(&positions, i);
		vec2f *uv = (vec2f *) list_get(&uvs, i);
		vertices[i].position.x = pos->x;
		vertices[i].position.y = pos->y;
		vertices[i].position.z = pos->z;
		vertices[i].uv.x = uv->x;
		vertices[i].uv.y = uv->y;
	
	}

	
	uint *indices = malloc(sizeof(uint) * faces.element_count);
	for (i = 0; i < faces.element_count; i++) {
		obj_face *face = list_get(&faces, i);
		indices[i] = face->position-1;


		int normal_index = face->normal-1;
		int vertex_index = face->position-1;
		vec3f *normal = (vec3f*) list_get(&normals, normal_index);
		vertices[vertex_index].normal.x = normal->x;
		vertices[vertex_index].normal.y = normal->y;
		vertices[vertex_index].normal.z = normal->z;
	}

	/*
	printf("Vertices:\n");
	for (i = 0; i < positions.element_count; i++) {
		vertex *v = &vertices[i];
		printf("%d: (%f %f %f), (%f %f), (%f %f %f)\n", i, v->position.x, v->position.y, v->position.z, v->uv.x, v->uv.y, v->normal.x, v->normal.y, v->normal.z);
	}

	printf("Indices:\n");
	for (i = 0; i < faces.element_count; i++) {
		printf("%d: %d\n", i, indices[i]);
	}
*/

	mesh_create(vertices, positions.element_count, indices, faces.element_count, M_RENDER_TYPE_3D);


	free(positions.elements);
	free(normals.elements);
	free(faces.elements);
	free(uvs.elements);

	free(src);
	printf("Finished creating mesh\n");
}


void world_render_mesh(mesh *mesh) {
	glBindVertexArray(mesh->vertex_array);
	glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
}

void world_render_type(mesh_render_type type) {
	uint i;
	for (i = 0; i < meshes[type].element_count; i++) {
		mesh *mesh = list_get(&meshes[type], i);
		glBindVertexArray(mesh->vertex_array);
		glDrawElements(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0);
	}
}


