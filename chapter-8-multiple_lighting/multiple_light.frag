#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;
	float constant;
	float linear;
	float quadratic;
	
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
	
	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};


uniform vec3 viewPos;
uniform Material material;
uniform DirLight dirLight;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

in vec3 fragPos;
in vec2 texCoords;
in vec3 normal;


vec3 CalcDirLight(DirLight light,vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

out vec4 fragColor;

void main()
{
	vec3 viewDir = normalize(viewPos - fragPos);
	
	vec3 result = CalcDirLight(dirLight,normal,viewDir);
	for(int i=0;i <NR_POINT_LIGHTS ; i++)
	{
		result+= CalcPointLight(pointLights[i],normal,fragPos,viewDir);
	}

	fragColor = vec4(result,1.0);//texture(material.diffuse,texCoords);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(viewDir,reflectDir),0),material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse,texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.specular,texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));
	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal, lightDir),0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(reflectDir, viewDir),0.0),material.shininess);
	
	float dis = length(lightDir);
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis * dis);
	
	vec3 ambient = light.ambient * vec3(texture(material.diffuse,texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular,texCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	
	float diff = max(dot(normal,lightDir),0.0);

	vec3 reflectDir = reflect(-lightDir,normal);
	float spec = pow(max(dot(reflectDir,viewDir),0.0),material.shininess);

	float dis = length(lightDir);
	float attenuation = 1.0 / (light.constant + light.linear * dis + light.quadratic * dis * dis);

	float spotCos = dot(lightDir,normalize(-lightDir));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((spotCos - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, texCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, texCoords));

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}
