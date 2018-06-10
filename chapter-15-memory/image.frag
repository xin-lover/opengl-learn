#version 330 core

uniform sampler2D image;

in vec2 texcoords;
out vec4 color;

void main()
{
	color = texture(image,texcoords);
}
