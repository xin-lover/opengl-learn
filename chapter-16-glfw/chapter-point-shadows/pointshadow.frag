#version 330 core

out vec4 color;

uniform samplerCube depthMap;
uniform vec3 lightPos;
uniform float far_plane;

in VS_OUT
{
    vec3 fragPos;
}fs_in;

void main()
{
    vec3 fragToLight = fs_in.fragPos - lightPos;
    float closestDepth = texture(depthMap,fragToLight).r;
    closestDepth *= far_plane;
    float currentDepth = length(fragToLight);
    float bias = 0.06;
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    color = vec4(1.0,0.0,0.0,1.0)* (1.0 - shadow);
    // color = vec4(vec3(closestDepth),1.0);
}