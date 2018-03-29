
#version 330 core

in vec2 vs_tex_coord;
out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex,vs_tex_coord);
	//color = vec4(1,0,0,1);
}
