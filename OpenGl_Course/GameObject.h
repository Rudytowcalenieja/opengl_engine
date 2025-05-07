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
	GameObject(const char* name);
	
	const char* GetName() { return displayName; }

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

	std::vector<std::unique_ptr<Component>> Components;
};

struct Rigidbody : public Component {

	void Update(GLfloat deltaTime) override {
		if (owner) {
			
		}
	}

};