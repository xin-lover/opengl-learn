#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 fragPos;
out vec3 normal;
out vec2 uv;

void main()
{
    vec4 viewPos = view * model * vec4(iPos,1.0);
    fragPos = viewPos.xyz;
    uv = iuv;
    normal = transpose(inverse(mat3(view * model))) * iNormal;
    gl_Position = projection * view * model * vec4(iPos,1.0);
}