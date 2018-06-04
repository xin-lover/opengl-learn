#version 330 core

uniform sampler2DArray tex;

in vec2 texcoords;
out vec4 color;

void main()
{
	color = texture(tex,vec3(texcoords.x,texcoords.y,0));
	//color = texture(tex,texcoords);
	//color = vec4(texcoords,0,1);
	//color = vec4(1,0,0,1);
}
