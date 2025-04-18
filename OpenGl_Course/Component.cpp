#pragma once

#include <glm/glm.hpp>
#include <iostream>

class GameObject;

struct Component {

	GameObject* owner = nullptr;

	virtual ~Component() = default;
	virtual void Update(GLfloat deltaTime) {
		
	}
};

struct BoxCollider3D : public Component {

	glm::vec3 bottomFrontLeft = glm::vec3(-0.5f, -0.5f, -0.5f);
	glm::vec3 bottomFrontRight = glm::vec3(0.5f, -0.5f, -0.5f);
	glm::vec3 bottomBackLeft = glm::vec3(-0.5f, -0.5f, 0.5f);
	glm::vec3 bottomBackRight = glm::vec3(0.5f, -0.5f, 0.5f);

	glm::vec3 topFrontLeft = glm::vec3(-0.5f, 0.5f, -0.5f);
	glm::vec3 topFrontRight = glm::vec3(0.5f, 0.5f, -0.5f);
	glm::vec3 topBackLeft = glm::vec3(-0.5f, 0.5f, 0.5f);
	glm::vec3 topBackRight = glm::vec3(0.5f, 0.5f, 0.5f);

	void Update(GLfloat deltaTime) override {

	}
};