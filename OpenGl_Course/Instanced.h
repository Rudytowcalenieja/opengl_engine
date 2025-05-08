#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Instanced
{
public:
	Instanced();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateInstanced();

	void Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	void Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale);
	void Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot);

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetRotation() { return rotation; }

	glm::mat4& GetModel() { return mat; }
	void SetModel(glm::mat4& m) { mat = m; }

	void RenderMesh();
	void ClearMesh();

	~Instanced();

private:
	const float toRadians = 0.017453292f;

	GLuint VAO, VBO, IBO, instancedVBO;
	GLsizei indexCount;

	bool isDisplayed;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 mat;

	std::vector<glm::mat4> matrixes;
};

