#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;
out vec3 normal;
out vec3 fragPos;

void main()
{
    gl_Position = projection * view * model * vec4(iPos,1.0);
    uv = vec2(iuv.x,1-iuv.y);
    normal = mat3(transpose(inverse(model))) * iNormal;
    // normal = iNormal;
    fragPos = vec3(model * vec4(iPos,1.0));
}