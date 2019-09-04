#version 430

layout(location=0) in vec4 color;
layout(location=1) in vec2 texCord;

out vec4 Color;

uniform sampler2D ourTexture;

void main()
{
	Color = texture(ourTexture, texCord);
}