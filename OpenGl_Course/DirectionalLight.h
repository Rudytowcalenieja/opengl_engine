#pragma once

#include "Light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();

	DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
					 GLfloat aIntensity, GLfloat dIntensity,
					 GLfloat xDir, GLfloat yDir, GLfloat zDir);

	void setAmbientIntensity(GLfloat aIntensity) {
		ambientIntensity = aIntensity;
	}

	void setDiffuseIntensity(GLfloat dIntensity) {
		diffuseIntensity = dIntensity;
	}

	void UseLight(GLuint aIntensityLoc, GLuint aColorLoc, GLuint dIntensityLoc, GLuint dLocation);

	~DirectionalLight() override;

private:
	glm::vec3 direction;
};

