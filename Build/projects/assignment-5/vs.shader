#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec2 texCord;
layout(location=2) in vec3 normal;

layout(location=0) out vec3 Color;
layout(location=1) out vec2 TexCord;
layout(location=2) out vec3 Normal;
layout(location=3) out vec3 FragPos;

uniform mat4 model;
uniform mat4 transform;

void main()
{
	gl_Position = transform * model * vec4(pos, 1.0f);
	TexCord = texCord;
	Normal = normal;
	FragPos = vec3(model * vec4(pos, 1.0));
}