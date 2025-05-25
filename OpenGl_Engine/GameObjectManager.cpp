#include "GameObjectManager.h"

GameObjectManager::GameObjectManager() {
	id = 0;
}

void GameObjectManager::CreateObject() {
	GameObject* object = new GameObject("GameObject " + id);
	id++;
}

void GameObjectManager::MakeInstanced() {
	// Something to do

	Instanced* instanced = new Instanced();
	instanced->NewInstance();
	instanced->CreateInstanced();

}

void GameObjectManager::Update() {
	// physics

	// updating

	// drawing
	for (auto& object : objectList) {
		object->Draw();
	}
}

GameObjectManager::~GameObjectManager() {

}
