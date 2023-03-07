#include <math.h>
#include "vecmath.h"
#include <stdlib.h>
#include <stdio.h>


void vec2f_mul_f(vec2f *a, float b, vec2f *out) {
	out->x = a->x * b;
	out->y = a->y * b;
}

void vec2f_sub(vec2f *a, vec2f *b, vec2f *out) {
	out->x = a->x - b->x;
	out->y = a->y - b->y;
}


void vec4_mul_f(vec4f *a, float b, vec4f *out) {
	out->x = a->x * b;
	out->y = a->y * b;
	out->z = a->z * b;
	out->w = a->w * b;
}

void vec4f_add(vec4f *a, vec4f *b, vec4f *out) {
	out->x = a->x + b->x;
	out->y = a->y + b->y;
	out->z = a->z + b->z;
	out->w = a->w + b->w;
}




void transform_from_position(vec3f *position, transform *transform) {
	transform->f[ 0] = 1.0f; // right
	transform->f[ 1] = 0.0f;
	transform->f[ 2] = 0.0f;
	transform->f[ 3] = 0.0f;
	transform->f[ 4] = 0.0f; // up
	transform->f[ 5] = 1.0f;
	transform->f[ 6] = 0.0f;
	transform->f[ 7] = 0.0f;
	transform->f[ 8] = 0.0f; // look
	transform->f[ 9] = 0.0f;
	transform->f[10] = 1.0f;
	transform->f[11] = 0.0f;
	transform->f[12] = position->x; // pos
	transform->f[13] = position->y;
	transform->f[14] = position->z;
	transform->f[15] = 1.0f;
}

void transform_print(transform *transform) {
	printf(
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n"
"%3.3f %3.3f %3.3f %3.3f\n",
		transform->f[ 0],
		transform->f[ 1],
		transform->f[ 2],
		transform->f[ 3],
		transform->f[ 4],
		transform->f[ 5],
		transform->f[ 6],
		transform->f[ 7],
		transform->f[ 8],
		transform->f[ 9],
		transform->f[10],
		transform->f[11],
		transform->f[12],
		transform->f[13],
		transform->f[14],
		transform->f[15]
	);
}

void transform_translate_f3(transform *transform, float x, float y, float z) {
	transform->pos.x += x;
	transform->pos.y += y;
	transform->pos.z += z;
}


void transform_translate_vec3f(transform *transform, vec3f p) {
	transform->pos.x += p.x;
	transform->pos.y += p.y;
	transform->pos.z += p.z;
}

void transform_scale_f(transform *transform, float s) {
	uint i;
	for (i = 0; i < 16; i++) {
		transform->f[i] *= s;
	}
}


void transform_rotation_reset(transform *transform) {
	transform->right.x = 1.0f;
	transform->right.y = 0.0f;
	transform->right.z = 0.0f;
	transform->right.w = 0.0f;
	
	transform->up.x = 0.0f;
	transform->up.y = 1.0f;
	transform->up.z = 0.0f;
	transform->up.w = 0.0f;

	transform->look.x = 0.0f;
	transform->look.y = 0.0f;
	transform->look.z = 1.0f;
	transform->look.w = 0.0f;
}

void transform_rotate_x(transform *transform, float r) {
	float c = cos(r);
	float s = sin(r);


	vec4f temp;

	vec4f look_new;
	vec4_mul_f(&transform->look, c, &look_new);
	vec4_mul_f(&transform->up, s, &temp);
	vec4f_add(&temp, &look_new, &look_new);	

	vec4f up_new;
	vec4_mul_f(&transform->up, c, &up_new);
	vec4_mul_f(&transform->look, -s, &temp);
	vec4f_add(&up_new, &temp, &up_new);

//       	= vec4f_add(
//		vec4_mul_f(transform->look, c),
//		vec4_mul_f(transform->up, s)
//	);
//
//	= vec4f_add(
//		vec4_mul_f(transform->up, c),
//		vec4_mul_f(transform->look, -s)
//	);
//
	transform->look = look_new;
	transform->up = up_new;
}


void transform_rotate_y(transform *transform, float r) {
	float c = cos(r);
	float s = sin(r);

	vec4f temp;

	vec4f right_new;	
	vec4_mul_f(&transform->right, c, &right_new);
	vec4_mul_f(&transform->look, s, &temp);
	vec4f_add(&temp, &right_new, &right_new);	

	vec4f look_new;
	vec4_mul_f(&transform->look, c, &look_new);
	vec4_mul_f(&transform->right, -s, &temp);
	vec4f_add(&look_new, &temp, &look_new);
/*	vec4f right_new = vec4f_add(
		vec4_mul_f(transform->right, c),
		vec4_mul_f(transform->look, s)
	);
	vec4f look_new = vec4f_add(
		vec4_mul_f(transform->look, c),
		vec4_mul_f(transform->right, -s)
	);*/
	transform->look = look_new;
	transform->right = right_new;
}


void transform_rotate_z(transform *transform, float r) {
	float c = cos(r);
	float s = sin(r);


	vec4f temp;

	vec4f up_new;	
	vec4_mul_f(&transform->up, c, &up_new);
	vec4_mul_f(&transform->right, s, &temp);
	vec4f_add(&temp, &up_new, &up_new);	

	vec4f right_new;
	vec4_mul_f(&transform->right, c, &right_new);
	vec4_mul_f(&transform->up, -s, &temp);
	vec4f_add(&right_new, &temp, &right_new);
/*
	vec4f up_new = vec4f_add(
		vec4_mul_f(transform->up, c),
		vec4_mul_f(transform->right, s)
	);
	vec4f right_new = vec4f_add(
		vec4_mul_f(transform->right, c),
		vec4_mul_f(transform->up, -s)
	);
*/
	transform->right = right_new;
	transform->up = up_new;
}
