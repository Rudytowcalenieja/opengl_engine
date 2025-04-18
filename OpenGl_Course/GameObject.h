#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "Component.cpp"

class GameObject : public Mesh
{
public:
	GameObject();
	GameObject(const char* name, bool displayed);
	GameObject(const char* name, bool displayed, GLfloat xPos, GLfloat yPos, GLfloat zPos);

	void Create(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xScale, GLfloat yScale, GLfloat zScale, GLfloat xRot, GLfloat yRot, GLfloat zRot);
	void Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	void Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale);
	void Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot);

	const char* GetName() { return displayName; }
	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetRotation() { return rotation; }

	template<typename T>
	T* AddComponent() {
		auto component = std::make_unique<T>();
		component->owner = this;
		T* ptr = component.get();
		Components.emplace_back(std::move(component));
		return ptr;
	}

	template<typename T>
	T* GetComponent() {
		for (auto& component : Components) {
			if (T* casted = dynamic_cast<T*>(component.get())) {
				return casted;
			}
		}
		return nullptr;
	}

	template<typename T>
	bool HasComponent() {
		for (auto& component : Components) {
			if (T* casted = dynamic_cast<T*>(component.get())) {
				return true;
			}
		}
		return false;
	}

	void UpdateAll(GLfloat deltaTime);

	~GameObject();

private:
	const char* displayName;
	bool isDisplayed;
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	std::vector<std::unique_ptr<Component>> Components;
};

struct Rigidbody : public Component {

	void Update(GLfloat deltaTime) override {
		if (owner) {
			std::cout << "X: " << owner->GetPosition().x << "Y: " << owner->GetPosition().y << "Z: " << owner->GetPosition().z << std::endl;
			owner->Translate(0.0f, 1.0f * deltaTime, 0.0f);
		}
	}

};