#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <typeinfo>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ImGui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "Light.h"
#include "Material.h"
#include "GameObject.h"

// Converting degress to radians
const float toRadians = 0.017453292f;

Window mainWindow;

// Lists
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
std::vector<GameObject*> gameObjectList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;

Material shinyMaterial;
Material dullMaterial;

Light mainLight;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat now = 0.0f;

// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";

void calcAverageNormals(unsigned int * indices, unsigned int indiceCount, GLfloat * vertices, unsigned int verticeCount, 
						unsigned int vLength, unsigned int normalOffset)
{
	unsigned int in0;
	unsigned int in1;
	unsigned int in2;
	glm::vec3 v1;
	glm::vec3 v2;
	glm::vec3 normal;

	for (size_t i = 0; i < indiceCount; i += 3)
	{
		in0 = indices[i] * vLength;
		in1 = indices[i + 1] * vLength;
		in2 = indices[i + 2] * vLength;
		v1 = glm::vec3(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		v2 = glm::vec3(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);
		
		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	unsigned int nOffset;
	glm::vec3 vec;

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		nOffset = i * vLength + normalOffset;
		vec = glm::vec3(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}

// Drawing triangle
void CreateObjects() {

	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
	//   x      y      z      u     v      nx    ny    nz
		-1.0f, -1.0f, -0.6f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -0.6f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(vertices, indices, 32, 12);
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
	//   x      y      z      u     v      nx    ny    nz
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f, // 0
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // 1
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f, // 2
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f, // 3
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f, // 4
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f, // 5
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 0.0f, // 6
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f  // 7
	};

	calcAverageNormals(cubeIndices, 36, cubeVertices, 64, 8, 5);

	Mesh* cube = new Mesh();
	cube->CreateMesh(cubeVertices, cubeIndices, 64, 36);
	meshList.push_back(cube);

	GameObject* test = new GameObject("Cube", true, 0.0f, 1.0f, 0.0f);
	test->CreateMesh(cubeVertices, cubeIndices, 64, 36);
	gameObjectList.push_back(test);
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

int main() {

	// Creating window
	mainWindow = Window(1600, 1200);
	mainWindow.Initialise();

	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");
	io.FontGlobalScale = 1.5f;

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.4f);
	camera.setWindow(mainWindow.getWindow());

	// Textures
	brickTexture = Texture("Images/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Images/dirt.png");
	dirtTexture.LoadTexture();

	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	mainLight = Light(1.0f,  1.0f,  1.0f, 0.1f,
					  0.0f, 0.0f, -1.0f, 0.3f);

	// Creating uniforms
	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0, uniformDirection = 0, uniformDiffuseIntensity = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	printf("Initialisation success!\n");

	printf("Created cube named %s\n", gameObjectList[0]->GetName());
	gameObjectList[0]->AddComponent<BoxCollider3D>();
	gameObjectList[0]->AddComponent<Rigidbody>();

	glm::vec3 cubePos(0.0f);

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {

		// deltaTime calculations
		now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		// Get & handle user input events
		glfwPollEvents();

		// Camera control
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Sky color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// Shaders
		shaderList[0].UseShader();
		// Getting uniforms
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformAmbientColor = shaderList[0].GetAmbientColorLocation();
		uniformAmbientIntensity = shaderList[0].GetAmbientIntensityLocation();
		uniformDirection = shaderList[0].GetDirectionLocation();
		uniformDiffuseIntensity = shaderList[0].GetDiffuseIntensityLocation();
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		// Light
		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor,
							uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		//
		// Drawing
		// 

		// First
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		brickTexture.UseTexture();
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[0]->RenderMesh();

		// Second
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 4.0f, -2.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dirtTexture.UseTexture();
		dullMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[1]->RenderMesh();

		gameObjectList[0]->UpdateAll(deltaTime);
		model = glm::mat4(1.0f);
		model = glm::translate(model, gameObjectList[0]->GetPosition());
		model = glm::scale(model, gameObjectList[0]->GetScale());
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		shinyMaterial.UseMaterial(uniformSpecularIntensity, uniformShininess);
		gameObjectList[0]->RenderMesh();
		
		glUseProgram(0);

		//
		// ImGui staff
		// //
		// ImGui Create frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::SetNextWindowSize(ImVec2(320, 0), ImGuiCond_Always);

		// ImGui Create window
		ImGui::Begin("Info");
		ImGui::LabelText("FPS", "%.1f", 1 / deltaTime);	
		ImGui::LabelText("Camera", "%.2f, %.2f, %.2f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		cubePos = gameObjectList[0]->GetPosition();
		ImGui::InputFloat("Cube X", &cubePos.x);
		ImGui::InputFloat("Cube Y", &cubePos.y);
		ImGui::InputFloat("Cube Z", &cubePos.z);
		ImGui::End();

		// ImGui Draw
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		mainWindow.swapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}