#version 330 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec2 in_tex_coord;

out vec2 vs_tex_coord;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	gl_Position = ProjectionMatrix * ModelViewMatrix * vec4(vPosition,1.0);
	vs_tex_coord = in_tex_coord * 2;
}
