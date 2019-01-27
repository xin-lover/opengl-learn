#version 330 core

layout(location=0) in vec3 iPos;

out vec3 texcoords;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    texcoords = iPos;
    vec4 pos = projection * view * vec4(iPos,1.0);
    gl_Position = pos.xyww;
}