#version 330 core

uniform samplerCube skybox;

out vec4 color;
in vec3 texcoords;

void main()
{
    color = texture(skybox,texcoords);
}