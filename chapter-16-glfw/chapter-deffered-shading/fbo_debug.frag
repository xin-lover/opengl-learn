#version 330 core

uniform sampler2D diffuseMap;

out vec4 color;
in vec2 uv;

void main()
{
    color = texture(diffuseMap,uv);    
}