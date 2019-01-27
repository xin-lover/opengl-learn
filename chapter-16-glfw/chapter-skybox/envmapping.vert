#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 worldPos;

void main()
{
    vec4 pos = model * vec4(iPos,1.0);
    worldPos = vec3(pos);
    gl_Position = projection * view * pos;
    normal = mat3(transpose(inverse(model))) * iNormal;
}