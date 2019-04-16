#version 330 core

in VS_OUT
{
    vec3 fragPos;
    vec4 fragPosInLightSpace;
    vec3 normal;
    vec2 texcoords;
}fs_in;

out vec4 color;
uniform sampler2D shadowmap;

void main()
{
    vec3 projCoords = fs_in.fragPosInLightSpace.xyz / fs_in.fragPosInLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    vec2 shadowuv = projCoords.xy;
    float dz = projCoords.z;
    float depth = texture(shadowmap,shadowuv).r;
    float bias = 0.005;
    if(depth > dz - bias)
    {
        color = vec4(1.0,0.0,0.0,1.0);
    }
    else
    {
        color = vec4(0.1,0.1,0.1,1.0);
    }
}