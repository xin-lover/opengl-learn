#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

out VS_OUT
{
    vec3 fragPos;
    vec3 normal;
    vec2 uv;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.fragPos = vec3(model * vec4(iPos,1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.normal = normalize(normalMatrix * iNormal);

    vs_out.uv = iuv;

    gl_Position = projection * view * model * vec4(iPos,1.0);
}