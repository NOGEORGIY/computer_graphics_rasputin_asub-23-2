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
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "Mesh.h"
#include "Model.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>


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
	
	glm::mat4 model = glm::mat4(1.0f);


	glm::mat4 rotateOX1 = glm::mat4(1.0f);
	glm::mat4 rotateOX2 = glm::mat4(1.0f);
	glm::mat4 rotateOX3 = glm::mat4(1.0f);

	float angleOX1 = 0.0f;
	float angleOX2 = 0.0f;
	float angleOX3 = 0.0f;
	


	shadering.uniform_set_vec(shader_program, "model", 1, &model[0][0], false, shadering.M4);
	
	Model modell("other.obj");

	
	glEnable(GL_DEPTH_TEST);
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
		const float detailSpeed = 0.5f;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPosition += cameraSpeed * cameraTarget;

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			cameraPosition += cameraSpeed * cameraTarget;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			cameraPosition -= cameraSpeed * cameraTarget;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			cameraPosition -= glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			cameraPosition += glm::normalize(glm::cross(cameraTarget, cameraUp)) * cameraSpeed;

		if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		{
			float newAngle = angleOX1 - detailSpeed;
			rotateOX1 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(0.0, 1.0, 0.0));
			angleOX1 = newAngle;
		}

		if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		{
			float newAngle = angleOX1 + detailSpeed;
			rotateOX1 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(0.0, 1.0, 0.0));
			angleOX1 = newAngle;
		}


		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		{
			if (angleOX2 > -95){
				float newAngle = angleOX2 - detailSpeed;
				rotateOX2 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0, 0.0, 0.0));
				angleOX2 = newAngle;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		{	
			if (angleOX2 < 95) {
				float newAngle = angleOX2 + detailSpeed;
				rotateOX2 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0, 0.0, 0.0));
				angleOX2 = newAngle;
			}
		}


		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		{
			if (angleOX3 > -120) {
				float newAngle = angleOX3 - detailSpeed;
				rotateOX3 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0, 0.0, 0.0));
				angleOX3 = newAngle;
			}
		}

		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		{	
			if (angleOX3 < 90) {
				float newAngle = angleOX3 + detailSpeed;
				rotateOX3 = glm::rotate(glm::mat4(1.0f), glm::radians(newAngle), glm::vec3(1.0, 0.0, 0.0));
				angleOX3 = newAngle;
			}
		}

		glm::mat4 view = glm::lookAt(
			cameraPosition,
			cameraPosition + cameraTarget,
			cameraUp);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shader_program);

		
		shadering.uniform_set_vec(shader_program, "model", 1, glm::value_ptr(model), GL_FALSE, shadering.M4);
		shadering.uniform_set_vec(shader_program, "view", 1, glm::value_ptr(view), GL_FALSE, shadering.M4);
		shadering.uniform_set_vec(shader_program, "projection", 1, glm::value_ptr(projection), GL_FALSE, shadering.M4);

		float timeValue = glfwGetTime();
		shadering.uniform_set_vec(shader_program, "in_color", 1 - sin(timeValue), cos(timeValue), sin(timeValue), 1.0f);
		glm::vec3 lightColor = glm::vec3(0.88005553535, 0.88005553535, 0.88005553535);
		glm::vec3 lightAmbient = lightColor * glm::vec3(0.1);
		glm::vec3 lightDiffuse = lightColor * glm::vec3(0.8);
		glm::vec3 lightSpecular = lightColor * glm::vec3(1.0);
		glm::vec3 lightPos = glm::vec3(1.0f, 1.0f, 1.0f);

		shadering.uniform_set_vec(shader_program, "material.ambient", 0.1f, 0.0f, 0.1f);
		shadering.uniform_set_vec(shader_program, "material.diffuse", 0.9f, 0.2f, 0.9f);
		shadering.uniform_set_vec(shader_program, "material.specular", 1.0f, 0.7f, 1.0f);
		shadering.uniform_set_vec(shader_program, "material.shininess", 64.0f);

		shadering.uniform_set_vec(shader_program, "light_1.ambient", lightAmbient[0], lightAmbient[1], lightAmbient[2]);
		shadering.uniform_set_vec(shader_program, "light_1.diffuse", lightDiffuse[0], lightDiffuse[1], lightDiffuse[2]);
		shadering.uniform_set_vec(shader_program, "light_1.specular", lightSpecular[0], lightSpecular[1], lightSpecular[2]);
		shadering.uniform_set_vec(shader_program, "light_1.position", lightPos[0], lightPos[1], lightPos[2]);
		shadering.uniform_set_vec(shader_program, "viewPos", cameraPosition[0], cameraPosition[1], cameraPosition[2]);

		modell.Draw(shadering, shader_program, rotateOX1, rotateOX2, rotateOX3);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwTerminate();
	return 0;
}
