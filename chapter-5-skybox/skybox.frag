
#version 330 core

in vec3 vs_tex_coord;
out vec4 color;

uniform samplerCube tex;

void main()
{
	color = texture(tex,vs_tex_coord);
}
