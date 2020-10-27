#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iuv;
layout(location=2) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(iPos,1.0);
}