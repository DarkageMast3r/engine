#version 330 core

layout(location = 0) out vec4 color;


in vec2 uv;
in float brightness;
in vec3 normal_frag;
uniform sampler2D image;

void main() {
	vec3 light_dir = normalize(vec3(-1.0f, -1.0f, -1.0f));
	
//	float b = dot(normalize(normal_frag), light_dir);

	color = vec4(vec3(max(0, brightness)), 1.0f);

//	color = vec4(uv, 0.0f, 1.0f);
//	color = texture(image, uv);
}
