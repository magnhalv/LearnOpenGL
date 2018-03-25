#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <assert.h>
#include "Shader.h"
#include "InputHandler.h"
#include "Camera.h";
#include "ResourceMananger.h"
#include <stb_image\stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), 2.5f);

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


glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

unsigned int rectangleIndices[] = {  // note that we start from 0!
	0, 1, 3,   // first triangle
	1, 2, 3    // second triangle
};

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

void processKeyboard(InputHandler &input, GLFWwindow *window) {
	if (input.keyTapped(GLFW_KEY_ESCAPE)) {
		glfwSetWindowShouldClose(window, true);
	}
	
	if (input.keyTapped(GLFW_KEY_UP)) {
		ratio = ratio >= 1 ? 1 : ratio + 0.1;
	}

	if (input.keyTapped(GLFW_KEY_DOWN)) {
		ratio = ratio <= 0 ? 0 : ratio - 0.1;
	}
	
	if (input.keyDown(GLFW_KEY_W))
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (input.keyDown(GLFW_KEY_S))
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (input.keyDown(GLFW_KEY_A))
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (input.keyDown(GLFW_KEY_D))
		camera.ProcessKeyboard(RIGHT, deltaTime);
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

float fov = 45.0f;

void applyClipMatrix(const Shader &shader) {					
	shader.setMat4("view", camera.GetView());

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), 800.0f/600.0f, 0.1f, 100.0f);
	shader.setMat4("projection", projection);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	camera.ProcessMouse(xpos, ypos);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void updateCameraSpeed() {
	float currentFrame = glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;	
}

int main() {
	auto window = createWindow(800, 600);
	
	Shader myShader = ResourceManager::LoadShader("./shaders/vertex.glsl", "./shaders/fragment.glsl", nullptr, "default");
	myShader.use();

	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	VBO = createVBO(rectangle, sizeof(rectangle), 5);
	//EBO = createEBO(rectangleIndices, sizeof(rectangleIndices));
	glBindVertexArray(0);
	
	auto texture1 = ResourceManager::LoadTexture("./textures/wood_container.jpg", false, "wood");
	auto texture2 = ResourceManager::LoadTexture("./textures/awesomeface.png", true, "face");
	

	glUniform1i(glGetUniformLocation(myShader.ID, "texture1"), 0);
	glUniform1i(glGetUniformLocation(myShader.ID, "texture2"), 1);	
	
	int ratioLocation = glGetUniformLocation(myShader.ID, "ratio");	

	InputHandler input(window);	

	glEnable(GL_DEPTH_TEST);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);	

	while (!glfwWindowShouldClose(window)) {

		updateCameraSpeed();
		glfwSetScrollCallback(window, scroll_callback);

		processKeyboard(input, window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		
	
		glUniform1f(ratioLocation, ratio);

		glActiveTexture(GL_TEXTURE0);		
		texture1.Bind();

		glActiveTexture(GL_TEXTURE1);
		texture2.Bind();

		applyClipMatrix(myShader);
		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = glm::radians(20.0f * i);

			if (i % 3 == 0) {
				angle = (float)glfwGetTime()*glm::radians(50.0f);
			}

			model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
			myShader.setMat4("model", model);

			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glDrawArrays(GL_TRIANGLES, 0, 36);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}