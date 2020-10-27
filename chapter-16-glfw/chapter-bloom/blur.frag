#version 330 core

in vec2 uv;
uniform sampler2D image;

uniform bool horizontal;
uniform float weights[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

out vec4 color;

void main()
{
    vec2 tex_offset = 1.0 / textureSize(image,0);
    vec3 result = texture(image,uv).rgb * weights[0];

    if(horizontal)
    {
        for(int i=1;i<5;++i)
        {
            result += texture(image,uv + vec2(tex_offset.x * i,0.0)).rgb * weights[i];
            result += texture(image,uv - vec2(tex_offset.x * i,0.0)).rgb * weights[i];
        }
    }
    else
    {
        for(int i=1;i<5;++i)
        {
            result += texture(image,uv + vec2(tex_offset.y * i,0.0)).rgb * weights[i];
            result += texture(image,uv - vec2(tex_offset.y * i,0.0)).rgb * weights[i];
        }
    }

    color = vec4(result,1.0);
}