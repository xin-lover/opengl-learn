
#version 330 core

in vec4 pcolor;
out vec4 color;

void main()
{
	//color = pcolor;
	color = vec4(vec3(gl_FragCoord.z),1.0);
}
