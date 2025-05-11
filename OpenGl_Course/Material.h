#pragma once

#include <GL/glew.h>

#include "Shader.h"

class Material
{
public:
	Material();
	Material(GLfloat sIntensity, GLfloat shine);

	void UseMaterial();

	~Material();

private:
	GLfloat specularIntensity;
	GLfloat shininess;
};

