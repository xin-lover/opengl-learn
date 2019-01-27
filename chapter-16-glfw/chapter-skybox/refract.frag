#version 330 core

in vec3 worldPos;
in vec3 normal;
out vec4 color;

uniform samplerCube skybox;
uniform vec3 cameraPos;

void main()
{
    float ratio = 1.0 / 1.33;
    vec3 I = normalize(worldPos - cameraPos);
    vec3 R = refract(I,normalize(normal),ratio);
    color = texture(skybox,R);
}