#include "Instanced.h"

Instanced::Instanced() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	instancedVBO = 0;
	indexCount = 0;

	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
}

void Instanced::CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numOfIndices) {
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

void Instanced::CreateInstanced() {	
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -10.0f, 1.0f));
	matrixes.push_back(model);
	matrixes.at(0) = glm::scale(matrixes.at(0), glm::vec3(10.0f, 1.0f, 10.0f));


	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, matrixes.size() * sizeof(glm::mat4), matrixes.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	std::size_t vec4Size = sizeof(glm::vec4);
	for (int i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
		glVertexAttribDivisor(4 + i, 1);
	}
	glBindVertexArray(0);
}

void Instanced::Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	position = glm::vec3(xPos, yPos, zPos);
}

void Instanced::Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale) {
	scale = glm::vec3(xScale, yScale, zScale);
}

void Instanced::Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	rotation = glm::vec3(xRot, yRot, zRot);
}

void Instanced::RenderMesh() {
	mat = glm::mat4(1.0f);

	mat = glm::scale(mat, scale);
	//mat = glm::rotate(mat, 3.14159256f, rotation);

	glUniform3f(Shader::GetUniform(1, "position"), position.x, position.y, position.z);
	glUniformMatrix4fv(Shader::GetUniform(1, "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, matrixes.size());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Instanced::ClearMesh() {
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

Instanced::~Instanced() {
	ClearMesh();
}