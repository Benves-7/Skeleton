#version 430

layout(location=0) in vec3 pos;
layout(location=1) in vec4 color;
layout(location=2) in vec2 texCord;

layout(location=0) out vec4 Color;
layout(location=1) out vec2 TexCord;

uniform mat4 transform;

void main()
{
	gl_Position = transform * vec4(pos, 1.0f);
	Color = color;
   TexCord = texCord;
}