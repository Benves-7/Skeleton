#version 430

layout(location=0) in vec3 color;
layout(location=1) in vec2 texCord;
layout(location=2) in vec3 normal;
layout(location=3) in vec3 fragPos;

out vec4 Color;

uniform sampler2D ourTexture;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
	// Ambient light
	float ambientStrength = 0.01;
	vec3 ambient = ambientStrength * lightColor;

	// Diffuse light
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(lightPos - fragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular Lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - fragPos);
	
	vec3 reflectDir = reflect(-lightDir, norm);

	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 64);
	vec3 specular = specularStrength * spec * lightColor;

	// attenuation light
	float distanceToLight = length(lightPos - fragPos);
	float attenuation = 1.0 / (1.0 + 0.01 * pow(distanceToLight, 2));

	vec3 result = ambient + (attenuation * (diffuse + specular));
	Color = texture(ourTexture, texCord) * vec4(result, 1.0);
}