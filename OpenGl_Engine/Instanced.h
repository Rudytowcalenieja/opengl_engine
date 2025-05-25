#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

struct transform {
	glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	glm::vec3 scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
	glm::mat4 model = glm::mat4(1.0f);
};

class Instanced
{
public:
	Instanced();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);

	void NewInstance();
	void CreateInstanced();

	void Translate(size_t index, GLfloat xPos, GLfloat yPos, GLfloat zPos);
	void Scale(size_t index, GLfloat xScale, GLfloat yScale, GLfloat zScale);
	void Rotate(size_t index, GLfloat xRot, GLfloat yRot, GLfloat zRot);

	transform& GetTransform(size_t index) {
		return transforms.at(index);
	}

	std::vector<transform> GetTransformList() { return transforms; }

	void RebuildMatrix(size_t index);
	void UpdateMatrix(size_t index);

	// Something here may has error
	/*glm::vec3 GetPosition(size_t index) { return instancedMatrices.at(index)[3]; }
	glm::vec3 GetScale(size_t index) {
		glm::vec3 tScale;
		tScale.x = glm::length(glm::vec3(instancedMatrices.at(index)[0]));
		tScale.y = glm::length(glm::vec3(instancedMatrices.at(index)[1]));
		tScale.z = glm::length(glm::vec3(instancedMatrices.at(index)[2]));
		return tScale;
	}
	glm::vec3 GetRotation(size_t index) { return glm::eulerAngles(glm::quat_cast(instancedMatrices.at(index))); }*/

	//glm::mat4& GetModel(size_t index) { return instancedMatrices.at(index); }
	void SetModel(size_t index, glm::mat4& m) { instancedMatrices.at(index) = m; }

	void RenderMesh();
	void ClearMesh();

	~Instanced();

private:
	const float toRadians = 0.017453292f;

	GLuint VAO, VBO, IBO, instancedVBO;
	GLsizei indexCount;

	bool isDisplayed;

	glm::mat4 mat;

	std::vector<glm::mat4> instancedMatrices;
	std::vector<transform> transforms;
};

