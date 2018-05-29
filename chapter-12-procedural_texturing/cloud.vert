#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat4 normalMat;

uniform vec3 lightPos;
uniform float scale;

layout(location = 0)in vec3 iPos;
layout(location = 2)in vec3 iNormal;

out float lightIntensity;
out vec3 mcPos;

void main()
{
	vec3 ecPos = vec3(view * model * vec4(iPos,1));
	mcPos = iPos * scale;
	vec3 tnorm = normalize(vec3(normalMat * vec4(iNormal,1.0)));
	vec3 lpos = vec3(view * model * vec4(lightPos,1.0));
	lightIntensity = dot(normalize(lpos - ecPos),tnorm);
	lightIntensity *= 1.5f;
	gl_Position = proj * view * model * vec4(iPos,1);
}

