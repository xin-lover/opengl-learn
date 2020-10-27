#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iuv;
layout(location=2) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection; 

out vec3 fragPos;
out vec3 normal;
out vec2 uv;

void main()
{
    uv = iuv;
    vec4 worldPos = model * vec4(iPos,1.0);
    fragPos = worldPos.xyz;
    normal = mat3(model) * iNormal;
    gl_Position =  projection * view * model * vec4(iPos,1.0);
}