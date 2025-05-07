#include "Terrain.h"

Terrain::Terrain() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	instancedVBO = 0;
	indexCount = 0;
}

void Terrain::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
	indexCount = numOfIndices;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numOfIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numOfVertices, vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Terrain::CreateInstanced() {

	const int width = 1000;
	const int height = 1000;

	std::vector<unsigned char> noiseData(width * height);

	float scale = 0.05f;  // Adjust to zoom in/out
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			float nx = x * scale;
			float ny = y * scale;
			float noise = stb_perlin_noise3(nx, ny, 0.0f, 0, 0, 0); // Z=0, no tiling
			noise = noise * 0.5f + 0.5f;  // Convert from [-1, 1] to [0, 1]
			noiseData[y * width + x] = static_cast<unsigned char>(noise * 10);
		}
	}

	int i = 0;
	for (int y = 0; y < 1000; ++y) {
		for (int x = 0; x < 1000; ++x) {
			offsets[i] = glm::vec3(x - 5, noiseData[i] * 1.0f, y - 5);
			i++;
		}
	}

	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * COUNT, offsets, GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(3, 1);
	glBindVertexArray(0);
}

void Terrain::RenderMesh() {
	mat = glm::mat4(1.0f);

	glUniform3f(Shader::GetUniform(1, "position"), 0, 0, 0);
	glUniformMatrix4fv(Shader::GetUniform(1, "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, COUNT);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Terrain::ClearMesh() {
	if (IBO != 0) {
		glDeleteBuffers(1, &IBO);
		IBO = 0;
	}

	if (VBO != 0) {
		glDeleteBuffers(1, &VBO);
		VBO = 0;
	}

	if (VAO != 0) {
		glDeleteVertexArrays(1, &VAO);
		VAO = 0;
	}

	if (instancedVBO != 0) {
		glDeleteBuffers(1, &instancedVBO);
		instancedVBO = 0;
	}

	indexCount = 0;
}

Terrain::~Terrain() {
	ClearMesh();
}