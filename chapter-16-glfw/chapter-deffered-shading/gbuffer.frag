#version 330 core

layout(location=0)out vec3 gPosition;
layout(location=1)out vec3 gNormal;
layout(location=2)out vec4 gAlbedoSpec;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform sampler2D tex_diffuse;

void main()
{
    gPosition = fragPos;
    gNormal = normalize(normal);
    gAlbedoSpec.rgb= texture(tex_diffuse,uv).rgb;
    gAlbedoSpec.a = 0.0;
}