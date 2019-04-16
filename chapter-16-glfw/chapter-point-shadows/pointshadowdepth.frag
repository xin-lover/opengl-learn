#version 330 core

in vec4 fragPos;
uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    float lightDis = length(fragPos.xyz - lightPos);

    lightDis = lightDis / far_plane;
    gl_FragDepth = lightDis;
}