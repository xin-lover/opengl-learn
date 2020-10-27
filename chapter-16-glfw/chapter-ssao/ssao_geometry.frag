#version 330 core

layout(location=0) out vec3 gPosition;
layout(location=1) out vec3 gNormal;
layout(location=2) out vec3 gAlbedo;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{
    vec4 c1 = texture(texture_diffuse1,vec2(0.0,0.0));
    vec4 c2 = texture(texture_specular1,vec2(0.0,0.0));
    gPosition = fragPos;
    gNormal = normalize(normal);
    gAlbedo = vec3(0.95);

}