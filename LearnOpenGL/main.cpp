#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
#include "Shader.h"
#include "InputHandler.h"
#include <stb_image\stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float ratio = 0;

float triangle[] = {
	// positions         // colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
};

unsigned int triangleIndices[] = {
	0, 1, 2
};

float rectangle_old[] = {
	// positions          // colors           // texture coords
	0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};


float rectangle[] = {
	// positions        // texture coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};


unsigned int rectangleIndices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(InputHandler &input, GLFWwindow *window) {
	if (input.keyPressed(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}

	if (input.keyPressed(GLFW_KEY_UP)) {
		ratio = ratio >= 1 ? 1 : ratio + 0.1;
	}

	if (input.keyPressed(GLFW_KEY_DOWN)) {
		ratio = ratio <= 0 ? 0 : ratio - 0.1;
	}
}

void setVboData(GLuint vbo, float *vertices, unsigned int length) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint createVBO(float *vertices, unsigned int length, unsigned int stride) {
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, length, vertices, GL_STATIC_DRAW);

	//Vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//// Colors
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3*sizeof(float)));
	//glEnableVertexAttribArray(1);
	
	//Textures
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return VBO;
}

GLuint createEBO(unsigned int *indices, int indicesLength) {
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesLength, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	return EBO;
}

GLFWwindow* createWindow(unsigned int xDim, unsigned int yDim) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(xDim, yDim, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		assert(false);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		assert(false);
	}

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glViewport(0, 0, xDim, yDim);
	return window;
}

GLuint generateTexture(const char *path, bool hasAlpha) {
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
	if (data) {
		GLuint dataType = hasAlpha ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, dataType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	return texture;
}

void applyClipMatrix(const Shader &shader) {

	glm::mat4 model;
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	int modelLoc = glGetUniformLocation(shader.ID, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	
	glm::mat4 view;	
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	int viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);
	int projectionLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

int main() {
	auto window = createWindow(800, 600);
	
	Shader myShader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
	myShader.use();

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	VBO = createVBO(rectangle, sizeof(rectangle), 5);
	//EBO = createEBO(rectangleIndices, sizeof(rectangleIndices));
	glBindVertexArray(0);
	
	auto texture1 = generateTexture("./textures/wood_container.jpg", false);
	auto texture2 = generateTexture("./textures/awesomeface.png", true);
	

	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);	
	
	int ratioLocation = glGetUniformLocation(myShader.ID, "ratio");	

	InputHandler input(window);	
	while (!glfwWindowShouldClose(window)) {

		processInput(input, window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);		
	
		glUniform1f(ratioLocation, ratio);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		applyClipMatrix(myShader);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}