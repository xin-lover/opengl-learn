#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main()
{
    uv = iuv;
    mat4 rotView = mat4(mat3(view));
    vec
    gl_Position = projection * rotView * model * vec4(iPos,1.0);
}