#version 330 core

in vec4 vPosition;

out vec4 pcolor;

uniform mat4 ModelViewMatrix;
uniform mat4 ProjectionMatrix;

void main()
{
	pcolor = vec4((vPosition.x + 0.5) / vPosition.w, (vPosition.y + 0.5) / vPosition.w, (vPosition.z + 0.5) / vPosition.w, 1);
	gl_Position = ProjectionMatrix * ModelViewMatrix * vPosition;
}
