#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader*> shaderList;
Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture cubeTexture;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

// Drawing triangle
void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//   x      y      z     u     v
		-1.0f, -1.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f, 0.5f, 0.0f,
		 1.0f, -1.0f,  0.0f, 1.0f, 0.0f,
		 0.0f,  1.0f,  0.0f, 0.5f, 1.0f
	};

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(vertices, indices, 20, 12);
	meshList.push_back(obj3);

	unsigned int cubeIndices[] = {
		1, 0, 2,
		1, 3, 2,
		5, 4, 6,
		5, 7, 6,
		4, 0, 2,
		2, 6, 4,
		1, 5, 7,
		7, 3, 1,
		0, 1, 5,
		5, 4, 0,
		2, 3, 7,
		7, 6, 2
	};

	GLfloat cubeVertices[] = {
	//   x      y      z     u     v
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 1
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, // 2
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f, // 3
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, // 4
		 0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 5
		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, // 6
		 0.5f,  0.5f,  0.5f, 0.0f, 1.0f  // 7
	};

	Mesh* cube = new Mesh();
	cube->CreateMesh(cubeVertices, cubeIndices, 40, 36);
	meshList.push_back(cube);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(shader1);
}

int main() {

	mainWindow = Window(1200, 900);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.4f);

	brickTexture = Texture("Images/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Images/dirt.png");
	dirtTexture.LoadTexture();
	cubeTexture = Texture("Images/cube_texture.png");
	cubeTexture.LoadTexture();

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(45.0f, mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	printf("Initialisation success!\n");

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {

		// deltaTime calculations
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get & handle user input events
		glfwPollEvents();

		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
		shaderList[0]->UseShader();
		uniformModel = shaderList[0]->GetModelLocation();
		uniformProjection = shaderList[0]->GetProjectionLocation();
		uniformView = shaderList[0]->GetViewLocation();

		// First
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 2.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		brickTexture.UseTexture();
		meshList[0]->RenderMesh();

		// Second
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -2.5f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		meshList[1]->RenderMesh();

		// Cube

		for (int i = 0; i <= 10; i++) {
			for (int j = 0; j <= 10; j++) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(i * 0.5f, 0.0f, j * 0.5f));
				model = glm::rotate(model, 0 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
				model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
				glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
				dirtTexture.UseTexture();
				meshList[3]->RenderMesh();
			}
		}

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(1.0f, 0.0f, -2.5f));
		model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		meshList[3]->RenderMesh();*/
		
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}