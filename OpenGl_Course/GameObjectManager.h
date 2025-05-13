#pragma once

#include <vector>

#include "GameObject.h"
#include "Instanced.h"

class GameObjectManager
{
public:
	GameObjectManager();
	void CreateObject();

	std::vector<GameObject*> GetObjectList() { return objectList; }

	void MakeInstanced();

	void Update();

	~GameObjectManager();

private:
	unsigned int id;
	std::vector<GameObject*> objectList;
};

