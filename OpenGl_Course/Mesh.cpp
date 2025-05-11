#include "Mesh.h"

Mesh::Mesh() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	indexCount = 0;

	matrix = glm::mat4(1.0f);
	scale = glm::vec3(1.0f);
	rotation = glm::vec3(0.0f);
}

void Mesh::CreateMesh(GLfloat* vertices, GLuint* indices, GLuint numOfVertices, GLuint numOfIndices) {
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
	
	// Disable i_Model attrib
	for (int i = 0; i < 4; ++i) {
		glDisableVertexAttribArray(4 + i);
	}
	// Set identity matrix column-by-column
	glVertexAttrib4f(4, 1.0f, 0.0f, 0.0f, 0.0f); // column 0
	glVertexAttrib4f(5, 0.0f, 1.0f, 0.0f, 0.0f); // column 1
	glVertexAttrib4f(6, 0.0f, 0.0f, 1.0f, 0.0f); // column 2
	glVertexAttrib4f(7, 0.0f, 0.0f, 0.0f, 1.0f); // column 3

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Mesh::Translate(GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	position = glm::vec3(xPos, yPos, zPos);
	matrix = glm::translate(glm::mat4(1.0f), position);
}

void Mesh::Scale(GLfloat xScale, GLfloat yScale, GLfloat zScale) {
	scale = glm::vec3(xScale, yScale, zScale);
	matrix = glm::scale(glm::mat4(1.0f), scale);
}

void Mesh::Rotate(GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	rotation = glm::vec3(xRot, yRot, zRot);
	matrix = glm::rotate(glm::mat4(1.0f), 360 * toRadians, rotation);
}

void Mesh::RenderMesh() {
	//matrix = glm::mat4(1.0f);

	//matrix = glm::scale(matrix, scale);
	//mat = glm::rotate(mat, 3.14159256f, rotation);

	//glUniform3f(Shader::GetUniform(1, "position"), position.x, position.y, position.z);
	glUniformMatrix4fv(Shader::GetUniform(1, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh() {
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

	indexCount = 0;
}

Mesh::~Mesh() {
	ClearMesh();
}