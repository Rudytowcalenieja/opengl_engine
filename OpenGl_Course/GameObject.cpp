#include "GameObject.h"

GameObject::GameObject() {
	displayName = "GameObject";
	isDisplayed = true;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(const char* name, bool displayed) {
	displayName = name;
	isDisplayed = displayed;
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

GameObject::GameObject(const char* name, bool displayed, GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	displayName = name;
	isDisplayed = displayed;
	position = glm::vec3(xPos, yPos, zPos);
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
}

void GameObject::Create(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xScale, GLfloat yScale, GLfloat zScale, GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	position = glm::vec3(xPos, yPos, zPos);
	scale = glm::vec3(xScale, yScale, zScale);
	rotation = glm::vec3(xRot, yRot, zRot);
}

void GameObject::Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	position += glm::vec3(xPos, yPos, zPos);
}

void GameObject::Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale) {
	scale += glm::vec3(xScale, yScale, zScale);
}

void GameObject::Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	rotation += glm::vec3(xRot, yRot, zRot);
}

void GameObject::UpdateAll(GLfloat deltaTime) {
	for (auto& component : Components) {
		component->Update(deltaTime);
	}
}


GameObject::~GameObject() {

}
