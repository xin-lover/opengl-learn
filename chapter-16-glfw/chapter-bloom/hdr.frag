#version 330 core

out vec4 color;
in vec2 uv;

uniform sampler2D hdrBuffer;
uniform sampler2D bloomBlur;
// uniform float exposure;


void main()
{
    const float gamma = 2.2;
    const float exposure = 1.0;
    vec3 hdrColor = texture(hdrBuffer,uv).rgb;
    vec3 bloomColor = texture(bloomBlur,uv).rgb;
    hdrColor += bloomColor;
    vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
    // result = pow(result,vec3(1.0/gamma));
    color = vec4(result,1.0);

    // color =texture(bloomBlur,uv);
    // color = texture(hdrBuffer,uv);
}