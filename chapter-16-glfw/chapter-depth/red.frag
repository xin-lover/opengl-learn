#version 330 core

in vec2 uv;
out vec4 color;

uniform float near;
uniform float far;
uniform sampler2D image;

float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));    
}

void main()
{
    // color = vec4(1.0,0.0,0.0,1.0);
    float depth = LinearizeDepth(gl_FragCoord.z) / far; // 为了演示除以 far
    color = vec4(vec3(depth),1.0);
    color = texture(image,uv);
}