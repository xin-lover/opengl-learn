#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices=3) out;

out vec4 fColor;

void main()
{
	/*gl_Position = gl_in[0].gl_Position + vec4(-0.1,0,0,0);
	fColor = vec4(1,0,0,1);
	EmitVertex();
	gl_Position = gl_in[0].gl_Position + vec4(0.1,0,0,0);
	fColor = vec4(0,1,0,1);
	EmitVertex();*/
	//if((gl_PrimitiveIDIn & 1) == 1)
	{
		int n;

		for(n =0; n < gl_in.length();n++)
		{
			gl_Position = gl_in[n].gl_Position;
			fColor = vec4(1,0,0,1);
			EmitVertex();
		}

		EndPrimitive();
	}

}
