#version 330 core

layout (location=0) in vec3 pos;
layout (location=1) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform mat4 normalMat;

out vec3 normal;
out vec3 fragPos;

void main()
{
	normal = normalize(mat3(model) * vNormal);

	fragPos = vec3(model * vec4(pos,1));
	gl_Position =  proj * view * model * vec4(pos,1);
}
