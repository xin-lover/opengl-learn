#version 330 core

out vec4 color;
in vec2 uv;

uniform sampler2D hdrBuffer;
// uniform float exposure;


void main()
{
    const float gamma = 2.2;
    const float exposure = 1.0;
    vec3 hdrColor = texture(hdrBuffer,uv).rgb;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // result = pow(result,vec3(1.0/gamma));
    color = vec4(result,1.0);

    // color = vec4(pow(hdrColor,vec3(1.0 / gamma)),1.0);
    // color = vec4(hdrColor * vec3(0.02),1.0);
}