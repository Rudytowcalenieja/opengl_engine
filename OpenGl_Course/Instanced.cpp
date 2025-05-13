#include "Instanced.h"

Instanced::Instanced() {
	VAO = 0;
	VBO = 0;
	IBO = 0;
	instancedVBO = 0;
	indexCount = 0;
	isDisplayed = true;

	mat = glm::mat4(1.0f);
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

void Instanced::NewInstance() {
	glm::mat4 model(1.0f);
	instancedMatrices.push_back(model);
}

void Instanced::CreateInstanced() {	
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, -10.0f, 1.0f));
	//instancedMatrices.push_back(model);
	//instancedMatrices.at(0) = glm::scale(instancedMatrices.at(0), glm::vec3(10.0f, 1.0f, 10.0f));


	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferData(GL_ARRAY_BUFFER, instancedMatrices.size() * sizeof(glm::mat4), instancedMatrices.data(), GL_STATIC_DRAW);

	glBindVertexArray(VAO);
	std::size_t vec4Size = sizeof(glm::vec4);
	for (int i = 0; i < 4; ++i) {
		glEnableVertexAttribArray(4 + i);
		glVertexAttribPointer(4 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(i * vec4Size));
		glVertexAttribDivisor(4 + i, 1);
	}
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Instanced::Translate(size_t index, GLfloat xPos, GLfloat yPos, GLfloat zPos) {
	//position = glm::vec3(xPos, yPos, zPos);
	//mat = glm::translate(glm::mat4(1.0f), position);
	//instancedMatrices.at(index) = glm::translate(glm::mat4(1.0f), position);
	instancedMatrices.at(index)[3].x = xPos;
	instancedMatrices.at(index)[3].y = yPos;
	instancedMatrices.at(index)[3].z = zPos;
	glm::vec3 temp = GetPosition(index);
	printf("Pos: %.1f %.1f %.1f\n", temp.x, temp.y, temp.z);
}

void Instanced::Scale(size_t index, GLfloat xScale, GLfloat yScale, GLfloat zScale) {
	//scale = glm::vec3(xScale, yScale, zScale);
	//mat = glm::scale(glm::mat4(1.0f), scale);
	//instancedMatrices.at(index) = glm::scale(glm::mat4(1.0f), scale);
	instancedMatrices.at(index)[0].x = xScale;
	instancedMatrices.at(index)[1].y = yScale;
	instancedMatrices.at(index)[2].z = zScale;
	glm::vec3 temp = GetScale(index);
	printf("Scale: %.1f %.1f %.1f\n", temp.x, temp.y, temp.z);
}

void Instanced::Rotate(size_t index, GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	//rotation = glm::vec3(xRot, yRot, zRot);
	//mat = glm::rotate(glm::mat4(1.0f), 360 * toRadians, rotation);
	//instancedMatrices.at(index) = glm::rotate(glm::mat4(1.0f), 360 * toRadians, rotation);
}

void Instanced::RenderMesh() {
	mat = glm::mat4(1.0f);

	//glUniform3f(Shader::GetUniform(1, "position"), position.x, position.y, position.z);
	glUniformMatrix4fv(Shader::GetUniform(1, "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instancedMatrices.size());
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