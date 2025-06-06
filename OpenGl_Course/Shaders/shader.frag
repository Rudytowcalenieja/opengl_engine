#version 330

in vec4 vColor;
in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 color;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

struct Material
{
	float specularIntensity;
	float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition;

vec4 ambientColor;
float diffuseFactor;
vec4 diffuseColor;
vec4 specularColor;
float specularFactor;
vec3 fragToEye;
vec3 reflectedVertex;

void main()
{
	ambientColor = vec4(directionalLight.color, 1.0f) * directionalLight.ambientIntensity;

	diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
	diffuseColor = vec4(directionalLight.color, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

	specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0.0f) {
		fragToEye = normalize(eyePosition - FragPos);
		reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));

		specularFactor = dot(fragToEye, reflectedVertex);
		if (specularFactor > 0.0f) {
			specularFactor = pow(specularFactor, material.shininess);
			specularColor = vec4(directionalLight.color * material.specularIntensity * specularFactor, 1.0f);
		}
	}

	color = texture(theTexture, TexCoord) * (ambientColor + diffuseColor + specularColor);
}