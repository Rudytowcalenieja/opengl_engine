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

void GameObject::Draw()
{
	o_Texture.UseTexture();
	o_Material.UseMaterial();
	RenderMesh();
}

//GameObject::GameObject(const char* name, bool displayed, GLfloat xPos, GLfloat yPos, GLfloat zPos) {
//	displayName = name;
//	isDisplayed = displayed;
//	position = glm::vec3(xPos, yPos, zPos);
//	scale = glm::vec3(1.0f, 1.0f, 1.0f);
//	rotation = glm::vec3(0.0f, 0.0f, 0.0f);
//}
//
//void GameObject::Create(GLfloat xPos, GLfloat yPos, GLfloat zPos, GLfloat xScale, GLfloat yScale, GLfloat zScale, GLfloat xRot, GLfloat yRot, GLfloat zRot) {
//	position = glm::vec3(xPos, yPos, zPos);
//	scale = glm::vec3(xScale, yScale, zScale);
//	rotation = glm::vec3(xRot, yRot, zRot);
//}
//
//void GameObject::SetPosition(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
//	position = glm::vec3(xPos, yPos, zPos);
//}
//
//void GameObject::SetScale(GLfloat xScale, GLfloat yScale, GLfloat zScale) {
//	scale = glm::vec3(xScale, yScale, zScale);
//}
//
//void GameObject::SetRotation(GLfloat xRot, GLfloat yRot, GLfloat zRot) {
//	rotation = glm::vec3(xRot, yRot, zRot);
//}
//
//void GameObject::Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
//	position += glm::vec3(xPos, yPos, zPos);
//}
//
//void GameObject::Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale) {
//	scale += glm::vec3(xScale, yScale, zScale);
//}
//
//void GameObject::Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot) {
//	rotation += glm::vec3(xRot, yRot, zRot);
//}

void GameObject::UpdateAll(GLfloat deltaTime) {
	for (auto& component : o_Components) {
		component->Update(deltaTime);
	}
}

GameObject::~GameObject() {

}
