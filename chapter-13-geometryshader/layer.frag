#version 410 core

in GS_FS_VERTEX
{
	vec4 color;
}vertex_in;

out vec4 color;

void main()
{
	color = vertex_in.color;
}
