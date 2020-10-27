#version 330 core

out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec4 c1 = texture(texture_diffuse1,vec2(0.0,0.0));
    vec4 c2 = texture(texture_specular1,vec2(0.0,0.0));
    color = vec4(0.9);
}