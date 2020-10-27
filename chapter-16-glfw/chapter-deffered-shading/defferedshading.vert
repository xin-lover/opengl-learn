#version 330 core

layout(location=0)in vec3 iPos;
layout(location=1)in vec2 iuv;

out vec2 uv;

void main()
{
    uv = iuv;
    gl_Position = vec4(iPos,1.0);
}