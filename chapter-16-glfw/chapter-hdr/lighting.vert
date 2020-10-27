#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
}vs_out;

void main()
{
    vs_out.fragPos = vec3(model*vec4(iPos,1.0));
    vs_out.texCoords = iuv;

    vec3 n = iNormal;

    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vs_out.normal = normalize(normalMatrix * n);

    gl_Position = projection * view * model * vec4(iPos,1.0);
}