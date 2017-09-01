#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include <GL/glew.h>

class Shader {
public:
	GLuint program;

	Shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
		std::string vsSource;
		std::string fsSource;
		std::ifstream vsFile;
		std::ifstream fsFile;

		try {
			std::stringstream vsStream;
			std::stringstream fsStream;

			vsFile.open(vertexPath);
			fsFile.open(fragmentPath);

			vsStream << vsFile.rdbuf();
			fsStream << fsFile.rdbuf();

			vsFile.close();
			fsFile.close();

			vsSource = vsStream.str();
			fsSource = fsStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR: Shader::File failed to be read" << std::endl;
		}

		const GLchar *vsCode = vsSource.c_str();
		const GLchar *fsCode = fsSource.c_str();

		GLint success;
		GLchar infolog[512];

		// Vertex shader initialization
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vsCode, NULL);
		glCompileShader(vertexShader);

		// ERROR CHECK: vertex shader
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
			std::cout << "ERROR: Shader::Vertex compilation failed.\n" << infolog << std::endl;
		}

		// Fragment shader initialization
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fsCode, NULL);
		glCompileShader(fragmentShader);

		// ERROR CHECK: fragment shader
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
			std::cout << "ERROR: Shader::Fragment compilation failed.\n" << infolog << std::endl;
		}

		// Shader program initialization
		program = glCreateProgram();
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		glLinkProgram(program);

		// ERROR CHECK: shader program
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(program, 512, NULL, infolog);
			std::cout << "ERROR: Shader::Program linking failed.\n" << infolog << std::endl;
		}

		// CLEANUP: vertex and fragment shader
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void use() {
		glUseProgram(program);
	}
};

#endif