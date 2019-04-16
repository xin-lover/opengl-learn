#version 330 core

layout(location=0) in vec3 iPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 fragPos;
}vs_out;

void main()
{
    vs_out.fragPos = vec3(model * vec4(iPos,1.0));
    gl_Position = projection * view * model * vec4(iPos,1.0);
}