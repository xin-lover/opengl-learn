#version 330 core

uniform vec3 ambient;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;
uniform float strength;

in vec3 normal;
in vec3 fragPos;
out vec4 color;

void main()
{
	vec3 lightDirection = normalize(lightPos - fragPos);
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDirection,normal);
	float diffuse = max(0.0,dot(normal,lightDirection));
	float specular = max(0.0,dot(viewDir,reflectDir));

	if(diffuse == 0.0)
	{
		specular = 0.0;
	}
	else
	{
		specular = pow(specular,shininess);
	}

	vec3 scatteredLight = ambient + lightColor * diffuse;
	vec3 reflectedLight = lightColor * specular * strength;

	vec3 rgb = min(scatteredLight * vec3(0.5f,0,0) + reflectedLight,vec3(1.0));
	color = vec4(rgb,1.0);
}
