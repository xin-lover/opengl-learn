#version 330 core

layout(location=0) in vec3 iPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
	mat4 ss = mat4(
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,-1,-1.5,1);
	vec3 p = iPos + vec3(0,-1,-1.5);
	gl_Position = proj * view  * model *  vec4(iPos,1);
	//gl_Position = vec4(iPos,1);
}
