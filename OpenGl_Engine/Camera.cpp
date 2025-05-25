#include "Camera.h"

Camera::Camera() {
	position = glm::vec3(0.0f, 2.0f, 0.0f);
	worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = -90.0f;
	pitch = 0.0f;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = 5.0f;
	turnSpeed = 0.4f;

	velocity = 0.0f;

	update();
}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed) {
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	movementSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	velocity = 0.0f;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime) {
	velocity = movementSpeed * deltaTime;

	if (cursorHidden) {
		// Front
		if (keys[GLFW_KEY_W]) {
			position += front * velocity;
		}

		// Back
		if (keys[GLFW_KEY_S]) {
			position -= front * velocity;
		}

		// Left
		if (keys[GLFW_KEY_A]) {
			position -= right * velocity;
		}

		// Right
		if (keys[GLFW_KEY_D]) {
			position += right * velocity;
		}

		// Up
		if (keys[GLFW_KEY_SPACE]) {
			position += worldUp * velocity;
		}

		// Down
		if (keys[GLFW_KEY_LEFT_SHIFT]) {
			position -= worldUp * velocity;
		}
	}

	// Enable cursor
	if (keys[GLFW_KEY_C]) {
		if (cursorHidden) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			cursorHidden = false;
			keys[GLFW_KEY_C] = false;
		}
		else {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			cursorHidden = true;
			keys[GLFW_KEY_C] = false;
		}
	}
}

void Camera::mouseControl(GLfloat xChange, GLfloat yChange) {
	if (cursorHidden) {
		xChange *= turnSpeed;
		yChange *= turnSpeed;

		yaw += xChange;
		pitch += yChange;

		if (pitch > 89.0f) {
			pitch = 89.0f;
		}

		if (pitch < -89.0f) {
			pitch = -89.0f;
		}

		update();
	}
}

glm::mat4 Camera::calculateViewMatrix() {
	return glm::lookAt(position, position + front, up);
}

void Camera::update() {
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera() {

}