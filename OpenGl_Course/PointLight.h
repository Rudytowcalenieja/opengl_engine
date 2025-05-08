#pragma once
#include "Light.h"

class PointLight : public Light
{
public:
	PointLight();
	PointLight(GLfloat red, GLfloat green, GLfloat blue,
			   GLfloat aIntensity, GLfloat dIntensity,
			   GLfloat xPos, GLfloat yPos, GLfloat zPos,
			   GLfloat con, GLfloat lin, GLfloat exp);

	void UseLight(GLuint aIntensityLoc, GLuint aColorLoc, GLuint dIntensityLoc, GLuint posLoc, GLuint conLoc, GLuint linLoc, GLuint, GLuint expLoc);

	~PointLight() override;

private:
	glm::vec3 position;

	GLfloat constant, linear, exponent;
};

