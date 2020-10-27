#version 330 core

out vec4 color;

in vec2 uv;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D ssao;

struct Light{
    vec3 position;
    vec3 color;

    float linear;
    float quadratic;
};

uniform Light light;

void main()
{
    vec3 fragPos = texture(gPosition,uv).rgb;
    vec3 normal = texture(gNormal,uv).rgb;
    vec3 diffuse = texture(gAlbedo,uv).rgb;
    float ambientOcclusion = texture(ssao,uv).r;

    vec3 ambient = vec3(diffuse * 0.3 * ambientOcclusion);
    ambient = diffuse * 0.3;

    vec3 lighting = ambient;
    vec3 viewDir = normalize(-fragPos);
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 diff = max(dot(lightDir,normal),0.0) * diffuse * light.color;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir,normal),0.0),16.0);
    vec3 specular = spec * light.color;
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (1.0 + light.linear * distance + light.quadratic * distance * distance);
    diff *= attenuation;
    specular *= attenuation;
    lighting += diff + specular;

    color = vec4(lighting,1.0);
}