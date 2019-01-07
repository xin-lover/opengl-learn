#version 330 core

out vec4 color;
in vec2 uv;
uniform sampler2D image;

void main()
{
    color = vec4(1.0,0.0,0.0,1.0);
    color = texture(image,uv);
}