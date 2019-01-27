#version 330 core

layout(location=0) in vec2 iPos;
layout(location=1) in vec2 iuv;

out vec2 uv;

void main()
{
    gl_Position = vec4(iPos.x,iPos.y,0.0,1.0);
    uv = iuv;
}