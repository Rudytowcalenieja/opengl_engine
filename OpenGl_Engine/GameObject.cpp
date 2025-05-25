#include "GameObject.h"

GameObject::GameObject() {
	o_Texture = Texture();
	o_Texture.LoadTexture();
	o_Name = "GameObject";
}

GameObject::GameObject(const char* name) {
	o_Texture = Texture();
	o_Texture.LoadTexture();
	o_Name = name;
}

void GameObject::AssignTexture(Texture tex) {
	o_Texture = tex;
	tex.LoadTexture();
}

void GameObject::Draw() {
	o_Texture.UseTexture();
	o_Material.UseMaterial();
	RenderMesh();
}

void GameObject::UpdateAll(GLfloat deltaTime) {
	for (auto& component : o_Components) {
		component->Update(deltaTime);
	}
}

GameObject::~GameObject() {

}
