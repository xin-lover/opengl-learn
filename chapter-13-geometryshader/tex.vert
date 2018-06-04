#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTexcoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 texcoords;

void main()
{
	texcoords = iTexcoord;
	gl_Position = proj * view * model * vec4(iPos,1);
}
