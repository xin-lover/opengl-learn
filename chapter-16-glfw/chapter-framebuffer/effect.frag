#version 330 core

uniform sampler2D tex;
in vec2 uv;

out vec4 color;

const float offset = 1.0 / 300.0;

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,offset),
        vec2(0.0f,offset),
        vec2(offset,offset),
        vec2(-offset,0.0),
        vec2(0.0,0.0),
        vec2(offset,0.0),
        vec2(-offset,-offset),
        vec2(0.0,-offset),
        vec2(offset,-offset)
    );

    // // edge detection
    // float kernel[9]=float[](
    //     1,1,1,
    //     1,-8,1,
    //     1,1,1
    // );

    //sharpen
    // float kernel[9]=float[](
    //     -1,-1,-1,
    //     -1,9,-1,
    //     -1,-1,-1
    // );

    //blur
    float kernel[9]=float[](
        1.0/16.0,2.0/16.0,1.0 / 16.0,
        2.0/16.0,4.0/16.0,2.0/16.0,
        1.0/16.0,2.0/16.0,1.0/16.0
    );

    vec3 sampleTex[9];
    for(int i=0;i<9;++i)
    {
        sampleTex[i] = vec3(texture(tex,uv+offsets[i]));
    }

    vec3 col = vec3(0.0);
    for(int i=0;i<9;++i)
    {
        col += sampleTex[i] * kernel[i];
    }

    color = vec4(col,1.0);

    //gray
    color = texture(tex,uv);
    float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
    color = vec4(average,average,average,1.0);
}