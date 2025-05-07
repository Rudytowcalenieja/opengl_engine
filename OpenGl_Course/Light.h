#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);

	void setAmbientIntensity(GLfloat aIntensity) {
		ambientIntensity = aIntensity;
	}

	void setDiffuseIntensity(GLfloat dIntensity) {
		diffuseIntensity = dIntensity;
	}

	void UseLight(GLuint aIntensityLoc, GLuint aColorLoc, GLuint dIntensityLoc, GLuint dLocation);

	~Light();

private:
	glm::vec3 color;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

