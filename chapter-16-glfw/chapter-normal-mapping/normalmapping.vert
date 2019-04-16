#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec3 iNormal;
layout(location=2) in vec2 iuv;
layout(location=3) in vec3 itangent;
layout(location=4) in vec4 ibitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
}vs_out;

uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    vs_out.uv = iuv;
    vs_out.fragPos = vec3(model*vec4(iPos,1.0));
    mat3 normalMatrix = transpose(inverse(mat3(model)));
    vec3 T = normalize(normalMatrix * itangent);
    vec3 N = normalize(normalMatrix * iNormal);
    T = normalize(T - dot(T,N) * N);
    vec3 B = cross(N,T);
    
    mat3 TBN = transpose(mat3(T,B,N));
    vs_out.tangentLightPos = TBN * lightPos;
    vs_out.tangentViewPos = TBN * viewPos;
    vs_out.tangentFragPos = TBN * vs_out.fragPos;
    gl_Position = projection * view * model * vec4(iPos,1.0);
}