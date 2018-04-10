#version 330 core

uniform vec3 ambient;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform float shininess;
uniform float strength;

uniform float constantAttenuation;
uniform float linearAttenuation;
uniform float quadraticAttenuation;

uniform vec3 coneDir;
uniform float spotCosCutoff;
uniform float spotExponent;

in vec3 normal;
in vec3 fragPos;
out vec4 color;

void main()
{
	vec3 norm = normalize(normal);
	vec3 lightDirection = (lightPos - fragPos);
	float lightDis = length(lightDirection);
	lightDirection = lightDirection / lightDis;

	//判断当前片元接受光照的强度
	float attenuation = 1.0 /
	(constantAttenuation + 
		linearAttenuation * lightDis +
		quadraticAttenuation * lightDis * lightDis);

	vec3 viewDir = normalize(viewPos - fragPos);
	float spotCos = dot(lightDirection,-coneDir);
	if(spotCos < spotCosCutoff)
	{
		attenuation = 0.0;
	}
	else
	{
		attenuation  *= pow(spotCos,spotExponent);
	}

	vec3 reflectDir = reflect(lightDirection,norm);
	float diffuse = max(0.0,dot(norm,lightDirection));
	float specular = max(0.0,dot(viewDir,reflectDir));

	if(diffuse == 0.0)
	{
		specular = 0.0;
	}
	else
	{
		specular = pow(specular,shininess);
	}

	vec3 scatteredLight = ambient + lightColor * diffuse * attenuation;
	vec3 reflectedLight = lightColor * specular * strength * attenuation;

	vec3 rgb = min(scatteredLight * vec3(0.5f,0,0) + reflectedLight,vec3(1.0));
	color = vec4(rgb,1.0);
}
