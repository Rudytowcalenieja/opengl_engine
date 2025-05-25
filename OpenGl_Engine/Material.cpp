#include "Material.h"

Material::Material() {
	specularIntensity = 0.0f;
	shininess = 0.0f;
}

Material::Material(GLfloat sIntensity, GLfloat shine) {
	specularIntensity = sIntensity;
	shininess = shine;
}

void Material::UseMaterial() {
	glUniform1f(Shader::GetUniform(SHADER_MAIN, "material.specularIntensity"), specularIntensity);
	glUniform1f(Shader::GetUniform(SHADER_MAIN, "material.shininess"), shininess);
}

Material::~Material() {

}