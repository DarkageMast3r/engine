#ifndef VECMATH_H
#define VECMATH_H

typedef struct {
	float x, y;
} vec2f;

typedef struct {
	float x, y, z;
} vec3f;

typedef struct {
	float x, y, z, w;
} vec4f;

typedef union {
	float f[16];
	struct {
		vec4f right;
		vec4f up;
		vec4f look;
		vec4f pos;
	};
} transform;


void vec2f_add(vec2f *a, vec2f *b, vec2f *out);
void vec2f_sub(vec2f *a, vec2f *b, vec2f *out);
void vec2f_neg(vec2f *a, vec2f *out);
void vec2f_mul_vec2f(vec2f *a, vec2f *b, vec2f *out);
void vec2f_mul_f(vec2f *a, float b, vec2f *out);
float vec2f_dot(vec2f *a, vec2f *b);

void vec3f_add(vec3f *a, vec3f *b, vec3f *out);
void vec3f_sub(vec3f *a, vec3f *b, vec3f *out);
void vec3f_neg(vec3f *a, vec3f *out);
void vec3f_mul_vec3f(vec3f *a, vec3f *b, vec3f *out);
void vec3f_mul_f(vec3f *a, float b, vec3f *out);
float vec3f_dot(vec3f *a, vec3f *b);

void vec4f_add(vec4f *a, vec4f *b, vec4f *out);
void vec4f_sub(vec4f *a, vec4f *b, vec4f *out);
void vec4f_neg(vec4f *a, vec4f *out);
void vec4f_mul_vec4f(vec4f *a, vec4f *b, vec4f *out);
void vec4f_mul_f(vec4f *a, float b, vec4f *out);
float vec4f_dot(vec4f *a, vec4f *b);

void transform_mul_transform(transform *a, transform *b, transform *out);
void transform_rotate_x(transform *a, float rotation);
void transform_rotate_y(transform *a, float rotation);
void transform_rotate_z(transform *a, float rotation);
void transform_rotation_reset(transform *a);
void transform_from_position(vec3f *position, transform *out);



#endif
