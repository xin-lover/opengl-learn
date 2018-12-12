#version 330 core

uniform sampler2D image;
out vec4 color;
in vec2 uv;

void main()
{
	//color = vec4(1.0,0.0,0.0,1.0);
	color = texture(image,uv);
	//color = vec4(uv,0.0,1.0);
}
