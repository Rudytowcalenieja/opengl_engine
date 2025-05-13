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
	/*glm::mat4 model(1.0f);
	instancedMatrices.push_back(model);*/
	transform trans;
	transforms.push_back(trans);
}

void Instanced::CreateInstanced() {	
	
	for (size_t i = 0; i < transforms.size(); i++) {
		RebuildMatrix(i);
	}

	glGenBuffers(1, &instancedVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	//glBufferData(GL_ARRAY_BUFFER, instancedMatrices.size() * sizeof(glm::mat4), instancedMatrices.data(), GL_STATIC_DRAW);
	std::vector<glm::mat4> matrices;
	for (transform trans : transforms) {
		matrices.push_back(trans.model);
	}
	glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), matrices.data(), GL_STATIC_DRAW);

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
	/*glm::vec4 &translation = instancedMatrices.at(index)[3];
	translation.x = xPos;
	translation.y = yPos;
	translation.z = zPos;*/
	transforms.at(index).position = { xPos, yPos, zPos };
}

void Instanced::Scale(size_t index, GLfloat xScale, GLfloat yScale, GLfloat zScale) {
	/*glm::mat4 &model = instancedMatrices.at(index);
	model[0].x = xScale;
	model[1].y = yScale;
	model[2].z = zScale;*/
	transforms.at(index).scale = { xScale, yScale, zScale };
}

void Instanced::Rotate(size_t index, GLfloat xRot, GLfloat yRot, GLfloat zRot) {
	/*glm::mat3 rotation;
	glm::vec3 scale = GetTransform(index).scale;
	rotation[0] = glm::vec3(rotation[0]) / scale.x;
	rotation[1] = glm::vec3(rotation[1]) / scale.y;
	rotation[2] = glm::vec3(rotation[2]) / scale.z;
	glm::quat rotationQuat = glm::quat_cast(rotation);
	glm::vec3 eulerAngles = glm::eulerAngles(rotationQuat);*/
	transforms.at(index).rotation = { xRot, yRot, zRot };
}

void Instanced::RebuildMatrix(size_t index) {
	glm::mat4 newModel = glm::mat4(1.0f);
	transform &trans = transforms.at(index);
	newModel = glm::translate(newModel, trans.position);
	newModel = glm::rotate(newModel, trans.rotation.x, glm::vec3(1, 0, 0));
	newModel = glm::rotate(newModel, trans.rotation.y, glm::vec3(0, 1, 0));
	newModel = glm::rotate(newModel, trans.rotation.z, glm::vec3(0, 0, 1));
	newModel = glm::scale(newModel, trans.scale);
	trans.model = newModel;
}

void Instanced::UpdateMatrix(size_t index) {
	glBindBuffer(GL_ARRAY_BUFFER, instancedVBO);
	glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &transforms.at(index).model);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Instanced::RenderMesh() {
	mat = glm::mat4(1.0f);

	//glUniform3f(Shader::GetUniform(1, "position"), position.x, position.y, position.z);
	glUniformMatrix4fv(Shader::GetUniform(1, "model"), 1, GL_FALSE, glm::value_ptr(mat));

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	//glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, instancedMatrices.size());
	glDrawElementsInstanced(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0, transforms.size());
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