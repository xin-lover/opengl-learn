#version 410 core

layout(location=0) in vec3 iPos;

//uniform mat4 model;
//uniform mat4 view;
//uniform mat4 proj;

void main()
{
	gl_Position = vec4(iPos,1.0);
}
