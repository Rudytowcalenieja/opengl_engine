#include "Material.h"

Material::Material() {
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial(GLuint sIntensityLoc, GLuint shineLoc) {
	glUniform1f(sIntensityLoc, specularIntensity);
	glUniform1f(shineLoc, shininess);
}

Material::~Material() {

}
