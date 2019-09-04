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
	vec3 lightDir = normalize(fragPos - lightPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	// Specular Lighting
	float specularStrength = 0.5;
	vec3 viewDir = normalize(-viewPos);

	float cosAngIncidence = dot(norm, lightDir);
	cosAngIncidence = clamp(cosAngIncidence, 0, 1);

	vec3 halfAngle = normalize(lightDir + viewDir);
	float blinnTerm = dot(norm, halfAngle);
	blinnTerm = clamp(blinnTerm, 0, 1);
	blinnTerm = cosAngIncidence != 0.0 ? blinnTerm : 0.0;
	blinnTerm = pow(blinnTerm, specularStrength);

	vec3 specular = specularStrength * blinnTerm * lightColor;

	// attenuation light
	float distanceToLight = length(lightPos - fragPos);
	float attenuation = 1.0 / (1.0 + 0.01 * pow(distanceToLight, 2));

	vec3 result = ambient + (attenuation * (diffuse + specular));
	Color = /*texture(ourTexture, texCord) **/ vec4(result, 1.0);
}