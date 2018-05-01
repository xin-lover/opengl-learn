#version 330 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexcoords;

out vec2 Texcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	Texcoords = aTexcoords;
	gl_Position = proj * view * model * vec4(aPos,1.0);
}
