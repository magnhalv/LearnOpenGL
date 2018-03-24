#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
public:
	Camera(glm::vec3 Position, glm::vec3 Front, glm::vec3 Up, float speed);
	void ProcessKeyboard(CameraMovement movement, float deltaTime);
	void ProcessMouse(double x, double y, bool constrainPitch);
	glm::mat4 GetView();
private:
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;

	glm::mat4 View;

	float MovementSpeed;

	bool firstMouse;
	float Yaw;
	float Pitch;
	float prevYaw;
	float prevPitch;

	void UpdateCameraVectors();
};

Camera::Camera(glm::vec3 Position, glm::vec3 Front, glm::vec3 Up, float speed) 
	: Position(Position), Front(Front), Up(Up), MovementSpeed(speed) {
	firstMouse = true;
	Yaw = -90.0f;
	Pitch = 0.0f;
	UpdateCameraVectors();
}

void Camera::ProcessKeyboard(CameraMovement movement, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	switch (movement) {
	case FORWARD:
		Position += velocity * Front;
		break;
	case BACKWARD:
		Position -= velocity * Front;
		break;
	case LEFT:
		Position -= velocity * glm::normalize(glm::cross(Front, Up));
		break;
	case RIGHT:
		Position += velocity * glm::normalize(glm::cross(Front, Up));
		break;
	}	
	UpdateCameraVectors();	
}

void Camera::ProcessMouse(double x, double y, bool constrainPitch = true) {
	if (firstMouse) {
		prevYaw = x;
		prevPitch = y;
		firstMouse = false;
	}

	float xoffset = x - prevYaw;
	float yoffset = prevPitch - y;
	
	prevYaw = x;
	prevPitch = y;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (Pitch > 89.0f)
		Pitch = 89.0f;
	if (Pitch < -89.0f)
		Pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	UpdateCameraVectors();
}

glm::mat4 Camera::GetView() {
	return View;
}

void Camera::UpdateCameraVectors() {
	View = glm::lookAt(Position, Position + Front, Up);
}


