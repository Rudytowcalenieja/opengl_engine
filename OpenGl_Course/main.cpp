#define STB_IMAGE_IMPLEMENTATION
#define STB_PERLIN_IMPLEMENTATION

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

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "ImGuizmo.h"

#include "Window.h"
#include "Mesh.h"
#include "Instanced.h"
#include "Terrain.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "GameObject.h"

Window mainWindow;

// Vectors
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;
std::vector<GameObject*> gameObjectList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture waterTexture;

Material shinyMaterial;
Material dullMaterial;

DirectionalLight mainLight;

int fps_count = 0;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
GLfloat now = 0.0f;

static int selected = 0;
static int instance = 0;
static ImGuizmo::OPERATION operation = ImGuizmo::TRANSLATE;
static ImGuizmo::MODE mode = ImGuizmo::LOCAL;

// Shaders
static const char* vShader = "Shaders/shader.vert";
static const char* fShader = "Shaders/shader.frag";

Terrain* terrain;
Instanced* instanced;

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

	GLuint indices[] = {
		1, 2, 0,
		3, 1, 0,
		0, 2, 3,
		1, 3, 2
	};

	GLfloat vertices[] = {
	//   x      y      z      u     v      nx    ny    nz
		-1.0f, -1.0f, -1.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, -1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f, -1.0f,  1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
		 0.0f,  1.0f,  0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
	};

	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	GameObject* obj1 = new GameObject("Test 1");
	obj1->CreateMesh(vertices, indices, 32, 12);
	obj1->Translate(0.0f, 0.0f, 0.0f);
	gameObjectList.push_back(obj1);

	GameObject* obj2 = new GameObject("Test 2");
	obj2->CreateMesh(vertices, indices, 32, 12);
	obj2->Translate(0.0f, 0.0f, 2.5f);
	gameObjectList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(vertices, indices, 32, 12);
	obj3->Translate(0.0f, 0.0f, 5.0f);
	meshList.push_back(obj3);

	GLuint cubeIndices[] = {
		// Front
		0,  1,  2,   2,  3,  0,
		// Back
		4,  5,  6,   6,  7,  4,
		// Left
		8,  9, 10,  10, 11,  8,
		// Right
		12, 13, 14,  14, 15, 12,
		// Bottom
		16, 17, 18,  18, 19, 16,
		// Top
		20, 21, 22,  22, 23, 20
	};

	GLfloat cubeVertices[] = {
		// Positions          // UVs        // Normals

		// Front (+Z)
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // 0
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 0.0f, 1.0f,  // 1
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // 2
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   0.0f, 0.0f, 1.0f,  // 3

		// Back (-Z)
		 0.5f, -0.5f, -0.5f,  0.0f, 0.0f,   0.0f, 0.0f, -1.0f, // 4
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   0.0f, 0.0f, -1.0f, // 5
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 0.0f, -1.0f, // 6
		 0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 0.0f, -1.0f, // 7

		// Left (-X)
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  // 8
		-0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f, 0.0f,  // 9
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // 10
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f, 0.0f,  // 11

		// Right (+X)
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // 12
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,  // 13
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   1.0f, 0.0f, 0.0f,  // 14
		 0.5f,  0.5f,  0.5f,  0.0f, 1.0f,   1.0f, 0.0f, 0.0f,  // 15

		// Bottom (-Y)
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,   0.0f, -1.0f, 0.0f, // 16
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,   0.0f, -1.0f, 0.0f, // 17
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,   0.0f, -1.0f, 0.0f, // 18
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,   0.0f, -1.0f, 0.0f, // 19

		// Top (+Y)
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 20
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,   0.0f, 1.0f, 0.0f,  // 21
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,   0.0f, 1.0f, 0.0f,  // 22
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,   0.0f, 1.0f, 0.0f   // 23
	};

	//calcAverageNormals(cubeIndices, 36, cubeVertices, 192, 8, 5);

	Mesh* cube = new Mesh();
	cube->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	meshList.push_back(cube);

	GameObject* test = new GameObject("Cube");
	test->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	test->Translate(0.0f, -2.0f, 0.0f);
	gameObjectList.push_back(test);

	GameObject* block = new GameObject("Block");
	block->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	block->Translate(0.0f, -4.0f, 0.0f);
	gameObjectList.push_back(block);

	terrain = new Terrain();
	terrain->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	terrain->CreateInstanced();

	instanced = new Instanced();
	instanced->CreateMesh(cubeVertices, cubeIndices, 192, 36);
	instanced->NewInstance();
	instanced->Translate(0, 0.0f, -10.0f, 0.0f);
	instanced->Scale(0, 10.0f, 1.0f, 10.0f);
	instanced->NewInstance();
	instanced->Translate(1, -10.f, -5.0f, 0.0f);
	instanced->CreateInstanced();
}

void CreateShaders() {
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void TransformUI(size_t index) {
	if (ImGui::TreeNodeEx("Transform Instance", ImGuiTreeNodeFlags_DefaultOpen)) {
		transform& trans = instanced->GetTransform(index);
		bool edited = false;
		edited |= ImGui::DragFloat3("Position", glm::value_ptr(trans.position), 0.1f);
		glm::vec3 degress = glm::degrees(trans.rotation);
		edited |= ImGui::DragFloat3("Rotation", glm::value_ptr(degress), 1.0f);
		edited |= ImGui::DragFloat3("Scale", glm::value_ptr(trans.scale), 0.1f);

		if (edited) {
			glm::vec3 rad = glm::radians(degress);
			trans.rotation = rad;
			instanced->RebuildMatrix(index);
			instanced->UpdateMatrix(index);
		}

		ImGui::TreePop();
	}
}

void DrawGUI() {

	static float fpsHistory[100] = {};
	static int fpsIndex = 0;

	fpsHistory[fpsIndex] = ImGui::GetIO().Framerate;
	fpsIndex = (fpsIndex + 1) % IM_ARRAYSIZE(fpsHistory);

	ImGui::SetNextWindowPos(ImVec2(0, 0));

	// ImGui Create window
	ImGui::Begin("Info", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::LabelText("FPS Counter", "%.1f", ImGui::GetIO().Framerate);
	ImGui::PlotLines("FPS History", fpsHistory, IM_ARRAYSIZE(fpsHistory), 0, nullptr, 0.0f, 200.0f);
	ImGui::LabelText("Camera", "%.2f, %.2f, %.2f", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
	ImGui::Text(" ");
	ImGui::Text("Objects");
	if (ImGui::BeginCombo("##1", gameObjectList.at(selected)->GetName())) {
		for (int i = 0; i < gameObjectList.size(); ++i) {
			bool is_selected = (selected == i);
			if (ImGui::Selectable(gameObjectList.at(i)->GetName(), is_selected)) {
				selected = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::Text("Operation:");
	if (ImGui::RadioButton("Translate", operation == ImGuizmo::TRANSLATE)) operation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", operation == ImGuizmo::ROTATE)) operation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", operation == ImGuizmo::SCALE)) operation = ImGuizmo::SCALE;
	ImGui::Text("Mode:");
	if (ImGui::RadioButton("Local", mode == ImGuizmo::LOCAL)) mode = ImGuizmo::LOCAL;
	ImGui::SameLine();
	if (ImGui::RadioButton("World", mode == ImGuizmo::WORLD)) mode = ImGuizmo::WORLD;
	ImGui::Text(" ");
	ImGui::Text("Instances");
	if (ImGui::BeginCombo("##2", ("Instance " + std::to_string(instance)).c_str())) {
		for (int i = 0; i < instanced->GetTransformList().size(); ++i) {
			bool is_selected = (instance == i);
			printf("%d", i);
			if (ImGui::Selectable(("Instance " + std::to_string(i)).c_str(), is_selected)) {
				instance = i;
			}
			if (is_selected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	TransformUI(instance);

	ImGui::End();
}

int main() {

	srand(time(NULL));

	// Creating window
	mainWindow = Window(1800, 1200);
	mainWindow.Initialise();

	// Init ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontFromFileTTF("font.ttf", 18);
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330");

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.4f);
	camera.setWindow(mainWindow.getWindow());

	// Textures
	brickTexture = Texture("Images/brick.png");
	brickTexture.LoadTexture();
	dirtTexture = Texture("Images/dirt.png");
	dirtTexture.LoadTexture();
	waterTexture = Texture("Images/water.png");
	waterTexture.LoadTexture();

	// Materials
	shinyMaterial = Material(1.0f, 32);
	dullMaterial = Material(0.3f, 4);

	gameObjectList[0]->AssignTexture(dirtTexture);
	gameObjectList[0]->AssignMaterial(dullMaterial);

	gameObjectList[1]->AssignTexture(brickTexture);
	gameObjectList[1]->AssignMaterial(shinyMaterial);

	gameObjectList[2]->AssignTexture(brickTexture);
	gameObjectList[2]->AssignMaterial(shinyMaterial);

	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
								 0.1f, 0.3f,
								 0.0f, 0.0f, -1.0f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0, uniformAmbientIntensity = 0, uniformAmbientColor = 0, uniformDirection = 0, uniformDiffuseIntensity = 0, uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(glm::radians(60.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 200.0f);

	printf("Initialisation success!\n");

	glm::mat4 model(1.0f);

	double fps_lastTime = glfwGetTime();

	// Loop until window closed
	while (!mainWindow.getShouldClose()) {

		// deltaTime calculations
		now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;

		/*fps_count++;
		if (now - fps_lastTime >= 0.1) {
			fps_count = 0;
			fps_lastTime = now;
		}*/

		// Get & handle user input events
		glfwPollEvents();

		// Camera control
		camera.keyControl(mainWindow.getKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		DrawGUI();

		// --- Gizmo logic here ---
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(io.DisplaySize);
		ImGui::Begin("##gizmo_bg", nullptr,
			ImGuiWindowFlags_NoInputs |
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoScrollbar |
			ImGuiWindowFlags_NoBackground);
		ImGui::End();
		ImGuizmo::BeginFrame();
		ImGuizmo::SetOrthographic(false);
		ImGuizmo::SetDrawlist(ImGui::GetForegroundDrawList());
		ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
		
		model = gameObjectList.at(selected)->GetModel();

		ImGuizmo::Manipulate(
			glm::value_ptr(camera.calculateViewMatrix()),
			glm::value_ptr(projection),
			operation, // or ROTATE, SCALE
			mode, // or WORLD
			glm::value_ptr(model)          // Your 4x4 float matrix
		);
		
		if (ImGuizmo::IsUsing()) {
			gameObjectList.at(selected)->SetModel(model);
		}

		ImGui::Render();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Sky color
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shaderList[0].UseShader();
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

		mainLight.UseLight(uniformAmbientIntensity, uniformAmbientColor,
							uniformDiffuseIntensity, uniformDirection);

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		
		
		for (auto& object : gameObjectList) {
			object->Draw();
		}

		dirtTexture.UseTexture();
		terrain->RenderMesh();
		instanced->RenderMesh();
		
		glUseProgram(0);

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		mainWindow.swapBuffers();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}