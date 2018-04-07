#version 330 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec2 uv;

out vec2 vs_uv;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	vs_uv = uv;
	gl_Position = ProjectionMatrix * ViewMatrix * vec4(vPosition,1);
}
