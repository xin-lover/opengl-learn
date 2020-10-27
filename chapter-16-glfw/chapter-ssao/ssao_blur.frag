#version 330 core

out float color;
in vec2 uv;

uniform sampler2D ssaoTex;

void main()
{
    vec2 texelSize = 1.0 / textureSize(ssaoTex,0);
    float res = 0.0;
    for(int x=-2;x < 2;++x)
    {
        for(int y=-2;y<2;++y)
        {
            vec2 offset = vec2(float(x),float(y)) * texelSize;
            res += texture(ssaoTex,uv + offset).r;
        }
    }

    color = res / (4.0*4.0);
}