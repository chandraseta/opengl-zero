#include <iostream>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>

#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Shader.h"

// Screen size
const GLint WIDTH = 1280, HEIGHT = 720;

/*
 * Main program
 */
int main() {
	std::cout << "Select shape to draw:" << std::endl;
	std::cout << "1. Line" << std::endl;
	std::cout << "2. Square" << std::endl;
	std::cout << "3. Octagon" << std::endl;
	std::cout << "4. Circle" << std::endl;
	std::cout << "5. Polygon" << std::endl;
	std::cout << "Selection: ";
	int choice;
	std::cin >> choice;
	while (choice < 1 || choice > 5) {
		std::cout << "Invalid input, value must be between 1 and 4 inclusive" << std::endl;
		std::cout << "Selection: ";
		std::cin >> choice;
	}
	int inVertice;
	if (choice == 5) {
		std::cout << "How many sides? (Max. 32) " << std::endl;
		std::cin >> inVertice;
		while (inVertice < 3 || inVertice > 32) {
			if (inVertice < 3) {
				std::cout << "Invalid input, there is no " << inVertice << " sided polygon." << std::endl;
			}
			else {
				std::cout << "Invalid input, maximum number of sided is 32" << std::endl;
			}
			std::cout << "How many sides? (Max. 32) " << std::endl;
			std::cin >> inVertice;
		}
	}

	// GLFW initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Window initialization
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL-Zero", nullptr, nullptr);
	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// GLEW initialization
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return -1;
	}
	glViewport(0, 0, screenWidth, screenHeight);

	Shader shader("core.vs", "core.fs");

	// Define vertices
	GLfloat vertices[200];
	int n_vertice;

	if (choice == 1) {
		n_vertice = 2;
		GLfloat temp_vertices[] = {
			0.0f, 0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
			0.0f, -0.5f, 0.0f,	0.0f, 0.0f, 1.0f
		};
		memcpy(vertices, temp_vertices, sizeof(vertices));
	}
	else if (choice == 2) {
		n_vertice = 4;
		GLfloat temp_vertices[] = {
			-0.3f, -0.5f, 0.0f,	1.0f, 0.0f, 0.0f,
			-0.3f, 0.5f, 0.0f,	0.0f, 1.0f, 0.0f,
			0.3f, 0.5f, 0.0f,	0.0f, 0.0f, 1.0f,
			0.3f, -0.5f, 0.0f,	0.0f, 0.0f, 0.0f
		};
		memcpy(vertices, temp_vertices, sizeof(vertices));
 	}
	else if (choice == 3) {
		n_vertice = 8;
		GLfloat temp_vertices[] = {
			-0.15f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f,
			-0.3f, -0.25f, 0.0f,	1.0f, 0.0f, 1.0f,
			-0.3f, 0.25f, 0.0f,		0.0f, 0.0f, 1.0f,
			-0.15f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f,
			0.15f, 0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.3f, 0.25f, 0.0f,		1.0f, 0.0f, 1.0f,
			0.3f, -0.25f, 0.0f,		0.0f, 0.0f, 1.0f,
			0.15f, -0.5f, 0.0f,		1.0f, 0.0f, 1.0f
		};
		memcpy(vertices, temp_vertices, sizeof(vertices));
	}
	else {
		int n_triangle;
		if (choice == 4) {
			n_triangle = 32;
		}
		else {
			n_triangle = inVertice;
		}
		float PI = 3.14159;
		float radius = 0.5f;
		n_vertice = n_triangle;
		
		for (int tri = 0; tri <= n_triangle; tri++) {
			int i = tri * 6;
			// Circle position
			vertices[i] = 0.0f + ((radius-0.2f) * cos(tri * PI * 2 / n_triangle));
			vertices[i + 1] = 0.0f + (radius * sin(tri * PI * 2 / n_triangle));
			vertices[i + 2] = 0.0f;

			// Circle color
			vertices[i + 3] = float(tri % 3) / float(3);
			vertices[i + 4] = float((tri + 1) % 3) / float(3);
			vertices[i + 5] = float((tri + 2) % 3) / float(3);
		}
	}

	// Vertex Buffer Object and Vertex Array Object initialization
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	// Runtime control
	float rotationModifier = 0.0f;
	while (!glfwWindowShouldClose(window)) {
		// Poll event
		glfwPollEvents();

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
			if (rotationModifier < 0) {
				rotationModifier = 0;
			}
			rotationModifier += 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
			if (rotationModifier > 0) {
				rotationModifier = 0;
			}
			rotationModifier -= 0.01f;
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
			rotationModifier = 0.0f;
		}

		// Set background color
		glClearColor(0.3f, 0.3f, 0.4f, 1.0f);
		
		// Refresh window
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw
		shader.use();

		glm::mat4 transform;
		transform = glm::rotate(transform, (GLfloat)glfwGetTime() * rotationModifier, glm::vec3(0.0f, 0.0f, 1.0f));

		GLint transformLocation = glGetUniformLocation(shader.program, "transform");
		glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transform));

		glBindVertexArray(VAO);
		if (choice == 1) {
			glDrawArrays(GL_LINE_LOOP, 0, n_vertice);
		}
		else {
			glDrawArrays(GL_TRIANGLE_FAN, 0, n_vertice);
		}
		glBindVertexArray(0);

		// Swap buffer
		glfwSwapBuffers(window);
	}

	// CLEANUP: VAO, VBO, GLFW
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glfwTerminate();

	return 0;
}