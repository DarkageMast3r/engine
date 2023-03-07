#version 330 core

layout(location = 0) out vec4 color;


in vec2 uv;
uniform sampler2D image;

void main() {
//	color = vec4(uv, 0.0f, 1.0f);
	color = texture(image, uv);
}
