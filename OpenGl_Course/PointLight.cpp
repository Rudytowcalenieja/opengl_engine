#include "PointLight.h"

PointLight::PointLight() : Light() {
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	linear = 0.0f;
	exponent = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat green, GLfloat blue,
					   GLfloat aIntensity, GLfloat dIntensity,
					   GLfloat xPos, GLfloat yPos, GLfloat zPos,
					   GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint aIntensityLoc, GLuint aColorLoc, GLuint dIntensityLoc, GLuint posLoc,								GLuint conLoc, GLuint linLoc, GLuint, GLuint expLoc)
{
	glUniform3f(aColorLoc, color.x, color.y, color.z);
	glUniform1f(aIntensityLoc, ambientIntensity);
	glUniform1f(dIntensityLoc, diffuseIntensity);

	glUniform3f(posLoc, position.x, position.y, position.z);
	glUniform1f(conLoc, constant);
	glUniform1f(linLoc, linear);
	glUniform1f(expLoc, exponent);
}

PointLight::~PointLight() {

}