#version 330 core

in VS_OUT{
    vec3 fragPos;
    vec3 normal;
    vec2 texCoords;
}fs_in;

struct Light{
    vec3 position;
    vec3 color;
};

uniform Light lights[4];
uniform sampler2D diffuseTex;
uniform vec3 viewPos;
out vec4 color;

void main()
{
    // color = vec4(1.0);
    vec3 objColor = texture(diffuseTex,fs_in.texCoords).rgb;
    vec3 normal = normalize(fs_in.normal);

    vec3 ambient = 0.0* objColor;

    vec3 lighting = vec3(0.0);
    for(int i=0;i<4;i++)
    {
        vec3 lightDir = normalize(lights[i].position - fs_in.fragPos);
        float diff = max(dot(lightDir, normal),0.0);
        vec3 diffuse = lights[i].color * diff * objColor;
        vec3 result = diffuse;

        float distance = length(fs_in.fragPos - lights[i].position);
        result *= (1.0 / (distance * distance));
        lighting += result;
    }

    color = vec4(ambient + lighting,1.0);

    // vec3 lightDir1 = normalize(lights[0].position - fs_in.fragPos);
    // float diff1 = max(dot(lightDir1, normal),0.0);
    // vec3 diffuse1 = lights[0].color * diff1 * objColor; 
    // float distance1 = length(fs_in.fragPos - lights[0].position);
    // // float d = (1.0 / (distance1 * distance1));
    // color = vec4(diffuse1,1.0);
}