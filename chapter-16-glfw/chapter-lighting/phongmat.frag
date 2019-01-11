#version 330 core

out vec4 color;
in vec2 uv;
in vec3 normal;
in vec3 fragPos;
// uniform sampler2D image;

//光照
struct Material
{
    // vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

uniform Material material;

struct Light{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;
uniform vec3 viewPos;

void main()
{
    vec3 ambient = light.ambient * vec3(texture(material.diffuse,uv));

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position- fragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse,uv));

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular,uv));

    vec3 res = (ambient + diffuse + specular);
    // res = specular;
    color = vec4(res,1.0);
    // color = texture(image,uv);
}