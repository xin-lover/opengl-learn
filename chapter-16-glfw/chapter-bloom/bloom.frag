#version 330 core

layout(location=0) out vec4 color;
layout(location=1) out vec4 brightColor;

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 uv;
}fs_in;

struct Light{
    vec3 position;
    vec3 color;
};

uniform Light lights[4]; 

uniform sampler2D diffuseMap;

void main()
{
    vec3 cc = texture(diffuseMap,fs_in.uv).rgb;
    vec3 normal = normalize(fs_in.normal);

    vec3 ambient = 0.0 * cc;
    vec3 lighting =vec3(0.0);

    for(int i=0;i<4;++i)
    {
        vec3 lightDir = normalize(lights[i].position - fs_in.fragPos);
        float diff = max(dot(normal,lightDir),0.0);
        vec3 result = diff * cc * lights[i].color;

        float distance = length(fs_in.fragPos - lights[i].position);
        result *= 1.0 / (distance * distance);
        lighting += result;
    }

    vec3 result = ambient + lighting;
    float brightness = dot(result,vec3(0.6784, 0.7686, 0.651));
    if(brightness > 1.0)
    {
        brightColor = vec4(result,1.0);
    }
    else
    {
        brightColor = vec4(0.0,0.0,0.0,1.0);
    }

    color = vec4(result,1.0);
}
