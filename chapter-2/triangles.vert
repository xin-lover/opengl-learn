#version 330 core

in vec4 vPosition;

uniform mat4 ModelViewProjectionMatrix;

void main()
{
	gl_Position = vPosition;
}
