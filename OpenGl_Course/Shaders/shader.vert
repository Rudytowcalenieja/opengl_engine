#version 330

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 i_Offset;
layout (location = 4) in mat4 i_Model;

out vec4 vColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 position;

void main()
{
	gl_Position = projection * view * model * i_Model * vec4(pos + position + i_Offset, 1.0f);

	vColor = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);

	TexCoord = tex;

	Normal = mat3(transpose(inverse(model))) * norm;

	FragPos = (model * vec4(pos, 1.0)).xyz;
}