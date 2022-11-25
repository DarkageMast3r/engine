#version 330 core

in vec2 position;
in vec2 uv_coord;
out vec2 uv;

uniform vec2 window_scale;

void main() {
	uv = uv_coord;
	
//	vec2 window_scale = vec2(1.0f);

	gl_Position = vec4(
		position * window_scale + vec2(-1.0f, 1.0f),
		1.0f, 1.0f
	);
}
