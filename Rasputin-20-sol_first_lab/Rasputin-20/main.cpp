#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: couldn't start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window;
	window = glfwCreateWindow(1024, 768, "Help", nullptr, nullptr);
	if (!window) {
		fprintf(stderr, "sob");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	GLenum ret = glewInit();
	if (GLEW_OK != ret) {
		fprintf(stderr, "ERROR: %s\n", glewGetErrorString(ret));
		return 1;
	}

	while(!glfwWindowShouldClose(window)){
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		//В методичке 1.0, 1.0, 1.0
		glClear(GL_COLOR_BUFFER_BIT);
		
		glColor3f(1.0f, 1.0f, 1.0f);
		glBegin(GL_QUADS);
			glVertex2f(-0.4f, -0.2f); 
			glVertex2f(-0.4f, 0.4f);
			glVertex2f(0.4f, 0.4f);
			glVertex2f(0.4f, -0.2f);
		glEnd();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
