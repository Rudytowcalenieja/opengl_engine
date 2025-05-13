#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Mesh
{
public:
	Mesh();

	void CreateMesh(GLfloat *vertices, GLuint *indices, GLuint numOfVertices, GLuint numOfIndices);

	void Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos);
	void Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale);
	void Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot);

	glm::vec3 GetPosition() { return position; }
	glm::vec3 GetScale() { return scale; }
	glm::vec3 GetRotation() { return rotation; }

	glm::mat4& GetModel() { return matrix; }
	void SetModel(glm::mat4& m) { matrix = m; }

	void RenderMesh();
	void ClearMesh();

	~Mesh();

private:
	const float toRadians = 0.017453292f;

	GLuint VAO, VBO, IBO;
	GLsizei indexCount;
	
	bool isDisplayed;

	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 matrix;
};

