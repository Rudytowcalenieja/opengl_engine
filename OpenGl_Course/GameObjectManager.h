#pragma once

#include <vector>
#include "GameObject.h"

class GameObjectManager
{
public:
	GameObjectManager();
	void CreateObject();

	std::vector<GameObject*> GetObjectList() { return objectList; }

	~GameObjectManager();

private:
	std::vector<GameObject*> objectList;
};

