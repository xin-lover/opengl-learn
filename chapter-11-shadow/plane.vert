#version 330 core

layout(location=0) in vec3 iPos;
layout(location=1) in vec2 iTexcoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 lightView;

out VS_OUT {
	vec3 fragPos;
	vec4 fragPosLightSpace;
}vs_out;

out vec2 texcoords;

void main()
{
	vs_out.fragPos = vec3(model * vec4(iPos,1.0));
	vs_out.fragPosLightSpace = lightView * vec4(vs_out.fragPos,1.0);
	texcoords = iTexcoords;
	gl_Position = proj * view * model * vec4(iPos,1.0);
}
