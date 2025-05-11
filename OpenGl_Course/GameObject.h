#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Component.cpp"

class GameObject : public Mesh
{
public:
	GameObject();
	GameObject(const char* name);

	// IDK what is that but make instanced rendering compatible with materials and textures, also make default texture and material
	//void Create(GLfloat* vertices, GLuint* indices, GLuint numOfVertices, GLuint numOfIndices);

	void AssignTexture(Texture tex);
	void AssignMaterial(Material mat) { o_Material = mat; }

	void Draw();
	
	const char* GetName() { return o_Name; }

	template<typename T>
	T* AddComponent() {
		auto component = std::make_unique<T>();
		component->owner = this;
		T* ptr = component.get();
		o_Components.emplace_back(std::move(component));
		return ptr;
	}

	template<typename T>
	T* GetComponent() {
		for (auto& component : o_Components) {
			if (T* casted = dynamic_cast<T*>(component.get())) {
				return casted;
			}
		}
		return nullptr;
	}

	template<typename T>
	bool HasComponent() {
		for (auto& component : o_Components) {
			if (T* casted = dynamic_cast<T*>(component.get())) {
				return true;
			}
		}
		return false;
	}

	void UpdateAll(GLfloat deltaTime);

	~GameObject();

private:
	Texture o_Texture;
	Material o_Material;

	const char* o_Name;

	std::vector<std::unique_ptr<Component>> o_Components;
};

struct Rigidbody : public Component {

	void Update(GLfloat deltaTime) override {
		if (owner) {
			
		}
	}

};