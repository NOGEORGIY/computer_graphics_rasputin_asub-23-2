#pragma once
#define GLEW_DLL
#define GLFW_DLL
#include <cstdio>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <fstream>
#include <string>
#include <string.h> 
class Shader_loader {
private:
public:
	//Собирает шейдеры в программу
	GLuint oneLinkProgram(std::string fragment_shader_path = "./fragment_shader.glsl",
		std::string vertex_shader_path = "./vertex_shader.glsl") {

		char* vert;
		char* frag;

		std::ifstream frag_file(fragment_shader_path);
		if (!frag_file.is_open()) {
			std::cerr << "Error with file fragment_shader!\n";
			return 1;
		}
		std::string frag_string((std::istreambuf_iterator<char>(frag_file)), std::istreambuf_iterator<char>());
		frag = new char[frag_string.size() + 1];
		strcpy_s(frag, frag_string.size() + 1, frag_string.c_str());
		frag_file.close();


		std::ifstream vert_file(vertex_shader_path);
		if (!vert_file.is_open()) {
			std::cerr << "Error with file vertex_shader!\n";
			return 1;
		}
		std::string vert_string((std::istreambuf_iterator<char>(vert_file)), std::istreambuf_iterator<char>());
		vert = new char[vert_string.size() + 1];
		strcpy_s(vert, vert_string.size() + 1, vert_string.c_str());
		vert_file.close();

		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vs, 1, &vert, NULL);
		glCompileShader(vs);

		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fs, 1, &frag, NULL);
		glCompileShader(fs);

		GLuint shader_program = glCreateProgram();

		glAttachShader(shader_program, fs);
		GLint success;
		glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(fs, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_FRAGMENT_SHADER, InfoLog);
		}

		glAttachShader(shader_program, vs);
		glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
		if (!success) {
			GLchar InfoLog[1024];
			glGetShaderInfoLog(vs, sizeof(InfoLog), NULL, InfoLog);
			fprintf(stderr, "Error compiling shader type %d: '%s'\n", GL_VERTEX_SHADER, InfoLog);
		}

		glLinkProgram(shader_program);
		return shader_program;
	}

	//Передает параметры для uniform-переменной
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform1f(vertexColorLocation, x);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform2f(vertexColorLocation, x, y);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y, float z) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform3f(vertexColorLocation, x, y, z);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, float x, float y, float z, float a) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform4f(vertexColorLocation, x, y, z, a);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y, GLint z, GLint a) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform4i(vertexColorLocation, x, y, z, a);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y, GLint z) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform3i(vertexColorLocation, x, y, z);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x, GLint y) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform2i(vertexColorLocation, x, y);
	}
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLint x) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		glUniform1i(vertexColorLocation, x);
	}

	enum Fv_things {
		FV1,
		FV2,
		FV3,
		FV4
	};

	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, GLfloat* value, Fv_things fv_thing) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		switch (fv_thing) {
		case FV1:
			glUniform1fv(vertexColorLocation, count, value);
			break;
		case FV2:
			glUniform2fv(vertexColorLocation, count, value);
			break;
		case FV3:
			glUniform3fv(vertexColorLocation, count, value);
			break;
		case FV4:
			glUniform4fv(vertexColorLocation, count, value);
			break;
		}
	}

	enum Iv_things {
		IV1,
		IV2,
		IV3,
		IV4
	};

	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, GLint* value, Iv_things iv_thing) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		switch (iv_thing) {
		case IV1:
			glUniform1iv(vertexColorLocation, count, value);
			break;
		case IV2:
			glUniform2iv(vertexColorLocation, count, value);
			break;
		case IV3:
			glUniform3iv(vertexColorLocation, count, value);
			break;
		case IV4:
			glUniform4iv(vertexColorLocation, count, value);
			break;
		}
	}

	enum Matrix_things {
		M2,
		M3,
		M4
	};
	void uniform_set_vec(GLuint shader_program, std::string attribute_name, GLsizei count, const GLfloat* value, GLboolean transpose, Matrix_things m_thing) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
		switch (m_thing) {
		case M2:
			glUniformMatrix2fv(vertexColorLocation, count, transpose, value);
			break;
		case M3:
			glUniformMatrix3fv(vertexColorLocation, count, transpose, value);
			break;
		case M4:
			glUniformMatrix4fv(vertexColorLocation, count, transpose, value);
			break;
		}
	}

	//Передает только адрес uniform-переменной
	float get_attribute(GLuint shader_program, std::string attribute_name) {
		const GLchar* gl_attribute_name = reinterpret_cast<const GLchar*>(attribute_name.c_str());
		float vertexColorLocation = glGetUniformLocation(shader_program, gl_attribute_name);
	}
};