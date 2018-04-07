#version 330 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec2 uv;

out vec3 vs_tex_coord;

uniform mat4 ProjectionMatrix;
uniform mat4 ViewMatrix;

void main()
{
	vs_tex_coord = vPosition;
	//gl_Position = /*ProjectionMatrix * ModelViewMatrix **/ vec4(vPosition,1);
	mat3 m3 = mat3(ViewMatrix);
	vec4 pos = ProjectionMatrix * mat4(m3) * vec4(vPosition,1);
	gl_Position  = pos.xyww;
}
