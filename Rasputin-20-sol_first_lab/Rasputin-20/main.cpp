#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <math.h>
#include "shader_loader.h"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"


GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
bool firstMouse = true;
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;
float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn) {

	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
}

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

	Shader_loader shadering;
	GLuint shader_program = shadering.oneLinkProgram();

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);  
	glEnableVertexAttribArray(0); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(0);

	
	glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraDirection = glm::normalize(cameraPosition - cameraTarget);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(cameraUp, cameraDirection));
	
	while (!glfwWindowShouldClose(window)) {

		const unsigned int SCR_WIDTH = 1024;
		const unsigned int SCR_HEIGHT = 768;

		float lastX = SCR_WIDTH / 2;
		float lastY = SCR_HEIGHT / 2;

		glm::vec3 front;
		front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		front.y = sin(glm::radians(pitch));
		front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraTarget = glm::normalize(front);

		glm::mat4 projection = glm::perspective(
			glm::radians(45.0f),
			(float)SCR_WIDTH / (float)SCR_HEIGHT,
			0.1f,
			100.0f);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		
		glfwSetCursorPosCallback(window, mouse_callback);

		const float cameraSpeed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPosition += cameraSpeed * cameraTarget;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPosition -= cameraSpeed * cameraTarget;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPosition -= glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPosition += glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;

		glm::mat4 view = glm::lookAt(
			cameraPosition,
			cameraPosition + cameraTarget,
			cameraUp);
		
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(shader_program);

		shadering.uniform_set_vec(shader_program, "projection", 1, &projection[0][0], false, shadering.M4);
		shadering.uniform_set_vec(shader_program, "view", 1, &view[0][0], false, shadering.M4);

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
