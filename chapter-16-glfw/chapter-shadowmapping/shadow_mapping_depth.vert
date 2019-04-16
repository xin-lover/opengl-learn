#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

out VS_OUT
{
    vec3 fragPos;
    vec4 fragPosInLightSpace;
    vec3 normal;
    vec2 texcoords;
}vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 lightSpaceMatrix;

void main()
{
    gl_Position = projection * view * model * vec4(iPos,1.0);
    vs_out.fragPos = vec3(model * vec4(iPos,1.0));
    vs_out.normal = transpose(inverse(mat3(model))) * iNormal;
    vs_out.texcoords = iuv;
    vs_out.fragPosInLightSpace = lightSpaceMatrix * vec4(vs_out.fragPos,1.0);
}