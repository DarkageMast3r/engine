#include <math.h>
#include "world.h"

#define PI 3.1415926535

vec2f camera_rotation = {0.0f, 0.0f};

float clamp_f(float val, float min, float max) {
	if (val < min) {
		return min;
	} else if (val > max) {
		return max;
	} else {
		return val;
	}
}

void camera_rotate(vec2f *rotation) {
	camera_rotation.x = clamp_f(camera_rotation.x - rotation->y, PI*-0.5f, PI*0.5f);
	camera_rotation.y = fmod(camera_rotation.y - rotation->x, PI*2);
	transform_rotation_reset(&camera);
	transform_rotate_y(&camera, camera_rotation.y);
	transform_rotate_x(&camera, camera_rotation.x);
}
