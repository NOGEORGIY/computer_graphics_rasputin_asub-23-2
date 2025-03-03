#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <math.h>
#include "shader_loader.h"

int main() {
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: couldn't start GLFW3\n");
		return 1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

	//Â VBO çàïèñûâàþòñÿ âåðøèíû
	//VAO íóæíî äëÿ îòðèñîâêè
	//EBO õðàíèò èíäåêñû âåðøèí, âûáèðàÿ êîòîðûå ìîæíî ðèñîâàòü ôèãóðû
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	GLfloat points[] = {
	-0.4f, -0.2f, 0.0f,
	-0.4f, 0.4f, 0.0f,
	0.4f, 0.4f, 0.0f,
	0.4f, -0.2f, 0.0f,
	};

	GLuint elements[] = {
	0, 1, 2,
	2, 3, 0
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STREAM_DRAW);

	//Çàãðóæàåò øåéäåðû èç ôàéëîâ è ïîäêëþ÷àåò èõ â ïðîãó
	Shader_loader shadering;
	GLuint shader_program = shadering.oneLinkProgram();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0); //Ñîîáùàåò êàê èíòåðïðåòèðîâàòü âåðøèííûå äàííûå. 
	glEnableVertexAttribArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(0);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);
		float timeValue = glfwGetTime();
		//"Ïðè ñîçäàíèè ðåíäåðà äîáàâèòü ýëåìåíòû äâèæåíèÿ äëÿ îáúåêòà"
		//Если анимация не видна, уменьшить делитель 
		points[0] = points[0] + cos(timeValue) / 20000;
		points[1] = points[1] + cos(timeValue) / 20000;
		points[3] = points[3] + cos(timeValue) / 20000;
		points[4] = points[4] + cos(timeValue) / 20000;
		points[6] = points[6] + cos(timeValue) / 20000;
		points[7] = points[7] + cos(timeValue) / 20000;
		points[9] = points[9] + cos(timeValue) / 20000;
		points[10] = points[10] + cos(timeValue) / 20000;
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

		shadering.uniform_set_vec(shader_program, "in_color", 1 - sin(timeValue), cos(timeValue), sin(timeValue), 1.0f);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}
