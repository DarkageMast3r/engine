#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv_coord;
layout (location = 2) in vec3 normal;
out vec2 uv;
out float brightness;
out vec3 normal_frag;


uniform vec3 cam_pos;
uniform vec3 cam_look;
uniform vec3 cam_right;
uniform vec3 cam_up;


void main() {
	vec3 light_dir = normalize(vec3(-1.0f, 1.0f, -1.0f));
	uv = uv_coord;
	brightness = dot(normal, light_dir);
	normal_frag = normal;
	
	vec3 pos_local = position - cam_pos;
	vec3 pos_view = vec3(
		dot(pos_local, cam_right),
		dot(pos_local, cam_up),
		dot(pos_local, cam_look)
	);

	gl_Position = vec4(
		pos_view.xy / abs(pos_view.z),
		pos_view.z / 256.0f - 1.0f, 1.0f
	);
}
