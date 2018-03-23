#pragma once

#include <GLFW/glfw3.h>
#include <map>

class InputHandler {
public:
	InputHandler(GLFWwindow *window);
	bool keyTapped(GLuint key);
	bool keyDown(GLuint key);
private:
	GLFWwindow *window;
	std::map<int, int> previousState;
};

InputHandler::InputHandler(GLFWwindow *w) {
	window = w;
}

bool InputHandler::keyTapped(GLuint key) {
	if (previousState.find(key) == previousState.end()) {
		previousState[key] = GLFW_RELEASE;
	}
	auto currentState = glfwGetKey(window, key);
	bool wasPressed = currentState == GLFW_PRESS && previousState[key] == GLFW_RELEASE;
	previousState[key] = currentState;

	return wasPressed;
}

bool InputHandler::keyDown(GLuint key) {
	return glfwGetKey(window, key) == GLFW_PRESS;
}
