#version 330 core

out vec4 color;
in vec2 uv;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light{
    vec3 position;
    vec3 color;

    float linear;
    float quadratic;
};

const int NR_LIGHTS=32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

void main()
{
    vec3 pos = texture(gPosition,uv).rgb;
    vec3 normal = texture(gNormal,uv).rgb;
    vec3 albedo = texture(gAlbedoSpec,uv).rgb;
    vec3 viewDir = normalize(viewPos - pos);

    vec3 lighting = albedo * 0.1;
    for(int i=0;i<NR_LIGHTS;++i)
    {
        vec3 lightDir = normalize(lights[i].position - pos);
        float diffuse = max(dot(lightDir,normal),0.0);

        vec3 halfDir = normalize(viewDir + lightDir);
        float spec = pow(max(dot(halfDir,normal),0.0),16.0);

        float distance = length(pos - lights[i].position);
        float attenuation = 1.0 / (1.0 + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        diffuse *= attenuation;
        spec *= attenuation;

        lighting += (diffuse * lights[i].color * albedo + spec);
    }

    color = vec4(lighting,1.0);
}