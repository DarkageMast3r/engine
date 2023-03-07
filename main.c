#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include "file.h"
#include "world.h"
#include "debug.h"
#include "image.h"


typedef unsigned int uint;

shader shader_cur;
int paused = 1;

char pressed[GLFW_KEY_LAST+1] = {0};
void keypress_func(GLFWwindow *window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		pressed[key] = 1;

		if (key == GLFW_KEY_ESCAPE) {
			paused = !paused;
			if (paused) {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);			
			} else {
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			}
		}
	} else if (action == GLFW_RELEASE) {
		pressed[key] = 0;
	}
}

int window_size_cur[2] = {1024, 1024};

void window_resize(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);
	window_size_cur[0] = width;
	window_size_cur[1] = height;

	int loc =  shader_get_uniform_location(&shader_cur, "window_scale");
	glUniform2f(
		loc,
		 1.0f / (float) width,
		-1.0f / (float) height
	);
}


int main(void) {

	if (!glfwInit()) {
		printf("Failed to initialize glfw\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(
		1024, 1024,
		"Platformer",
		0, NULL
	);

	if (window == NULL) {
		glfwTerminate();
		printf("Failed to create window\n");
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keypress_func);

	
	if (glfwRawMouseMotionSupported()) {
		glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
	}


	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("Failed to initialize glad\n");
		return -1;
	}

	// Init program
	mesh_init();


	char *vertex_shader_source = file_read("assets/shaders/vertex.glsl");
	char *fragment_shader_source = file_read("assets/shaders/fragment.glsl");

	shader_cur = *shader_create_vf(
		vertex_shader_source,	
		fragment_shader_source
	);
	free(vertex_shader_source);
	free(fragment_shader_source);

	char *vertex3d_shader_source = file_read("assets/shaders/vertex3d.glsl");
	char *fragment3d_shader_source = file_read("assets/shaders/fragment3d.glsl");
	shader shader_3d = *shader_create_vf(
		vertex3d_shader_source,
		fragment3d_shader_source
	);
	free(vertex3d_shader_source);
	free(fragment3d_shader_source);


	image *image_letters = image_load("assets/images/letters.png");
	{
		char text[] = "hello world!";

		
		float x = 0.0f;
		uint i = 0;
		for (i = 0; text[i]; i++) {
			x = (float) i;

			uint index_count = 6;
			uint vertex_count = 4;
	
			float letter = (float) (text[i] - 'a');
			if (text[i]== ' ') {
				letter = 27.0f;
			}

			float min = letter / 27.0f;
			float max = (letter+1.0f) / 27.0f;
			vertex vertices[4] = {
				{{(x       ) * 64.0f,  64.0f, 1.0f}, {min, 0.0f}},
				{{(x + 1.0f) * 64.0f,  64.0f, 1.0f}, {max, 0.0f}},
				{{(x       ) * 64.0f,  0.0f,  1.0f}, {min, 1.0f}},
				{{(x + 1.0f) * 64.0f,  0.0f,  1.0f}, {max, 1.0f}}
			};
		
			uint indices[6] = {
				0, 1, 2,
				1, 2, 3
			};
	
			mesh_create(vertices, vertex_count, indices, index_count, M_RENDER_TYPE_2D);
		}
	}

	{
		vertex vertices[8] = {
			{{-1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}},
			{{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f}, { 1.0f, -1.0f, -1.0f}},
			{{-1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, {-1.0f,  1.0f, -1.0f}},
			{{ 1.0f,  1.0f, -1.0f}, {0.0f, 1.0f}, { 1.0f,  1.0f, -1.0f}},
			{{-1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, {-1.0f, -1.0f,  1.0f}},
			{{ 1.0f, -1.0f,  1.0f}, {0.0f, 0.0f}, { 1.0f, -1.0f,  1.0f}},
			{{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, {-1.0f,  1.0f,  1.0f}},
			{{ 1.0f,  1.0f,  1.0f}, {0.0f, 1.0f}, { 1.0f,  1.0f,  1.0f}},
		};

		uint indices[12 * 3] = {
			0, 1, 2,
			1, 2, 3,
			4, 5, 6,
			5, 6, 7,
			1, 3, 5,
			3, 5, 7,
			0, 2, 4,
			2, 4, 6
		};

//		mesh_create(vertices, 8, indices, 24, M_RENDER_TYPE_3D);
//		mesh_create_from_file_obj("assets/meshes/test.obj");
//		mesh_create_from_file_obj("assets/meshes/untitled.obj");
//		mesh_create_from_file_obj("assets/meshes/sphere.obj");
//		mesh_create_from_file_obj("assets/meshes/spherelow.obj");
//		mesh_create_from_file_obj("assets/meshes/simple.obj");
		mesh_create_from_file_obj("assets/meshes/suzanne.obj");
	}






	glUseProgram(shader_cur.shader_id);


	int uniform_image_location = glGetUniformLocation(shader_cur.shader_id, "image");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, image_letters->texture_name);
	glUniform1i(uniform_image_location, 0);


//	glUseProgram(shader_3d->shader_id);
//	int uniform_cam_location = uniform_get_location(shader_3d, "cam_pos");
//	glUniform3f(uniform_cam_location, 0.0f, 0.0f, -3.0f);

	glfwSetFramebufferSizeCallback(window, window_resize);
	
	printf("Setup correctly\n\n");
	vec2f mouse_coords;

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		if (!paused) {
			// update
			float dt = 1.0f / 60.0f;
	
			float forward = (pressed[GLFW_KEY_W] - pressed[GLFW_KEY_S]) * dt;
			float up = (pressed[GLFW_KEY_SPACE] - pressed[GLFW_KEY_LEFT_SHIFT]) * dt;
			float right = (pressed[GLFW_KEY_D] - pressed[GLFW_KEY_A]) * dt;
	
			camera.pos.x += right;
			camera.pos.y += up;
			camera.pos.z += forward;

			double mouse_x, mouse_y;
			glfwGetCursorPos(window, &mouse_x, &mouse_y);
			vec2f mouse_coords_new = {mouse_x, mouse_y};
			vec2f movement;
			vec2f_sub(&mouse_coords_new, &mouse_coords, &movement);
			vec2f_mul_f(&movement, 0.01f, &movement);
			camera_rotate(&movement);

			mouse_coords = mouse_coords_new;
		}





		// render
		glUseProgram(shader_3d.shader_id);
		
		int loc_cam_pos = shader_get_uniform_location(&shader_3d, "cam_pos");
		int loc_cam_look = shader_get_uniform_location(&shader_3d, "cam_look");
		int loc_cam_right = shader_get_uniform_location(&shader_3d, "cam_right");
		int loc_cam_up = shader_get_uniform_location(&shader_3d, "cam_up");
		glUniform3f(loc_cam_pos, camera.pos.x, camera.pos.y, camera.pos.z);
		glUniform3f(loc_cam_look, camera.look.x, camera.look.y, camera.look.z);
		glUniform3f(loc_cam_right, camera.right.x, camera.right.y, camera.right.z);
		glUniform3f(loc_cam_up, camera.up.x, camera.up.y, camera.up.z);




		glEnable(GL_DEPTH_TEST);
		glDepthRange(0.0f, 1.0f);
		world_render_type(M_RENDER_TYPE_3D);	


		glUseProgram(shader_cur.shader_id);
		int loc = shader_get_uniform_location(&shader_cur, "window_scale");	

		glUniform2f(loc,
			 1.0f / (float) window_size_cur[0],
			-1.0f / (float) window_size_cur[1]
		);



		glDisable(GL_DEPTH_TEST);
		world_render_type(M_RENDER_TYPE_2D);


	

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}
