#ifndef WORLD_H
#define WORLD_H

#include "util.h"
#include "vecmath.h"

typedef unsigned int uint;

typedef enum {
	M_RENDER_TYPE_2D,
	M_RENDER_TYPE_3D,
	M_RENDER_TYPE_COUNT
} mesh_render_type;

typedef struct {
	char *name;
	int location;
} uniform;

typedef struct {
	uint shader_id;
	list uniforms;
} shader;

typedef struct {
	vec3f position;
	vec2f uv;
	vec3f normal;
} vertex;

typedef struct {
	vertex *vertices;
	uint *indices;
	uint buffer_index, buffer_vertex;
	uint vertex_array;
	uint vertex_count, index_count;
} mesh;


extern transform camera;

void mesh_init();

mesh *mesh_create(vertex *vertices, uint vertex_count, uint *indices, uint index_count, mesh_render_type type);
void mesh_create_from_file_obj(char *path);
void world_render_type(mesh_render_type type);
void world_render_mesh(mesh *mesh);

shader *shader_create_vf(char *vertex_source, char *fragment_source);
int shader_get_uniform_location(shader *shader, char *uniform_name);

void camera_rotate(vec2f *rotation);


#endif
