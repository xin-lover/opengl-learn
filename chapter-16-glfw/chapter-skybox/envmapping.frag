#version 330 core

uniform samplerCube skybox;
uniform vec3 cameraPos;

out vec4 color;
in vec3 normal;
in vec3 worldPos;

void main()
{
    vec3 I = normalize(worldPos - cameraPos);
    vec3 R = reflect(I,normalize(normal));
    color = texture(skybox,R);
    // color = vec4(normalize(normal),1.0);
    // color = vec4(R,1.0);
}