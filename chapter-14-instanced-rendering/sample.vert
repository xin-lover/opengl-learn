#version 330 core

layout(location = 0) in vec3 iPos;
layout(location = 1) in vec3 iColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec4 fColor;

void main()
{
	fColor = vec4(iColor,1);
	vec3 pos = iPos;
	pos = pos + vec3(0.1,0.2f,-0.1) * gl_InstanceID;
	gl_Position= proj * view * model * vec4(pos,1);
}
