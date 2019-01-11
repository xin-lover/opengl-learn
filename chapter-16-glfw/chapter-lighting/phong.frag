#version 330 core

out vec4 color;
// in vec2 uv;
in vec3 normal;
in vec3 fragPos;
// uniform sampler2D image;

//光照
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    float ambientStength = 0.1;
    vec3 ambient = ambientStength * lightColor;

    vec3 objColor = vec3(1.0,0.0,0.0);

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm,lightDir),0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 res = (ambient + diffuse + specular) * objColor;
    color = vec4(res,1.0);
    // color = texture(image,uv);
}