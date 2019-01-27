#version 330 core

in vec2 uv;
uniform sampler2D tex;

out vec4 color;

void main()
{
    color = texture(tex,uv);
    // color = vec4(uv.x,uv.y,0.0,1.0);
}