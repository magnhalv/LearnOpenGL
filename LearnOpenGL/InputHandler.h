#pragma once

#include <GLFW/glfw3.h>
#include <map>

class InputHandler {
public:
	InputHandler(GLFWwindow *window);
	bool keyPressed(GLuint key);
private:
	GLFWwindow *window;
	std::map<int, int> previousState;
};

InputHandler::InputHandler(GLFWwindow *w) {
	window = w;
}

bool InputHandler::keyPressed(GLuint key) {
	if (previousState.find(key) == previousState.end()) {
		previousState[key] = GLFW_RELEASE;
	}
	auto currentState = glfwGetKey(window, key);
	bool wasPressed = currentState == GLFW_PRESS && previousState[key] == GLFW_RELEASE;
	previousState[key] = currentState;

	return wasPressed;
}
