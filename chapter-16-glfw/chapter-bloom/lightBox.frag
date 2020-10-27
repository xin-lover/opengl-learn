#version 330 core

layout(location=0) out vec4 color;
layout(location=1) out vec4 brightColor;

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 uv;
}fs_in;

uniform vec3 lightColor;

void main()
{
    color = vec4(lightColor,1.0);

    float brightness = dot(color.rgb,vec3(0.2126,0.7152,0.0722));
    if(brightness > 1.0)
    {
        brightColor = vec4(color.rgb,1.0);
    }
    else
    {
        brightColor = vec4(0.0,0.0,0.0,1.0);
    }
}