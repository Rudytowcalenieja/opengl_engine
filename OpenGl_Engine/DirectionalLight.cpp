#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() : Light() {
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat green, GLfloat blue,
								   GLfloat aIntensity, GLfloat dIntensity,
								   GLfloat xDir, GLfloat yDir, GLfloat zDir) :
	Light(red, green, blue,
		  aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLuint aIntensityLoc, GLuint aColorLoc, GLuint dIntensityLoc, GLuint dLocation) {
	glUniform3f(aColorLoc, color.x, color.y, color.z);
	glUniform1f(aIntensityLoc, ambientIntensity);

	glUniform3f(dLocation, direction.x, direction.y, direction.z);
	glUniform1f(dIntensityLoc, diffuseIntensity);
}

DirectionalLight::~DirectionalLight() {

}