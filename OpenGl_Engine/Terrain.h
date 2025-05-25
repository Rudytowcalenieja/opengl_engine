#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_perlin.h"

#include "Shader.h"

class Terrain
{
public:
	Terrain();

	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices);
	void CreateInstanced();

	void RenderMesh();
	void ClearMesh();

	~Terrain();

private:
	const float toRadians = 0.017453292f;

	const static int COUNT = 1000000;

	GLuint VAO, VBO, IBO, instancedVBO;
	GLsizei indexCount;

	glm::mat4 mat;

	glm::vec3 offsets[COUNT];
};