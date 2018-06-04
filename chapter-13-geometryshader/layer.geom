#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices=128) out;

in VS_GS_VERTEX
{
	vec4 color;
}vertex_in[];

out GS_FS_VERTEX
{
	vec4 color;
}vertex_out;

uniform mat4 proj;
uniform int output_slices;

//out in gl_Layer;

void main()
{
	for(int j =0; j < output_slices;++j)
	{
		gl_Layer = j;
		for(int i = 0; i < gl_in.length(); ++i)
		{
			gl_Position = proj * gl_in[i].gl_Position;
			vertex_out.color = vertex_in[i].color;
			EmitVertex();
		}
		EndPrimitive();
	}

	/*int i,j;
	mat4 slice_mat;
	float alpha = 0;
	float delta = float(output_slices - 1) * 0.25 * 3.1415927;

	for(j = 0; j < output_slices; ++j)
	{
		float s = sin(j * delta);
		float c = cos(j * delta);
		slice_mat = mat4(vec4(c,-s,0,0),
		vec4(s,c,0,0),
		vec4(0,0,1,0),
		vec4(0,0,0,1));
		slice_mat = proj * slice_mat ;
		
		gl_Layer = j;
		for(i = 0;i < gl_in.length();++i)
		{
			gl_Position = slice_mat * gl_in[i].gl_Position;
			vertex_out.color = vertex_in[i].color;
			EmitVertex();
		}
		EndPrimitive();
	}*/

	/*int i;
	float delta = float(output_slices - 1) * 0.25 * 3.1415927;
	float s = sin(1 * delta);
	float c = cos(1 * delta);
	mat4 slice_mat = mat4(vec4(c,-s,0,0),
			vec4(s,c,0,0),
			vec4(0,0,1,0),
			vec4(0,0,0,1));
	slice_mat = proj * slice_mat;
	for(i = 0; i < gl_in.length(); ++i)
	{
		gl_Position = slice_mat * gl_in[i].gl_Position;
		vertex_out.color = vertex_in[i].color;
		EmitVertex();
	}
	EndPrimitive();*/
}
