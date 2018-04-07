#version 330 core

uniform vec4 in_color;
out vec4 color;

void main()
{
	color = in_color;
	//color = vec4(vec3(gl_FragCoord.z),1);
}
