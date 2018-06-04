#version 410 core

layout(location=0) in vec3 iPos;

out VS_GS_VERTEX
{
	vec4 color;
}vertex_out;

void main()
{
	vertex_out.color = vec4(iPos,1);
	gl_Position = vec4(iPos,1);
}
