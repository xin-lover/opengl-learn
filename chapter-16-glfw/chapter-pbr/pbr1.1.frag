#version 330 core

out vec4 color;

in vec3 fragPos;
in vec3 normal;
in vec2 uv;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform vec3 lightPositions[4];
uniform vec3 lightColors[4];

uniform vec3 camPos;

const float PI=3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
    return nom / max(denom,0.001);
}

float GeometrySchlickGGX(float NdotV,float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N,V),0.0);    
    float NdotL = max(dot(N,L),0.0);
    float ggx1 = GeometrySchlickGGX(NdotV,roughness);
    float ggx2 = GeometrySchlickGGX(NdotL,roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta,vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta,5.0);
}

void main()
{
    vec3 N = normalize(normal);
    vec3 V = normalize(camPos - fragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0,albedo,metallic);

    vec3 Lo = vec3(0.0);
    for(int i=0;i<4;++i)
    {
        vec3 L = normalize(lightPositions[i] - fragPos);
        vec3 H = normalize(V + L);

        float distance = length(lightPositions[i] - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        float NDF = distributionGGX(N,H, roughness);
        float G = GeometrySmith(N,V,L,roughness);
        vec3 F = fresnelSchlick(clamp(dot(H,V),0.0,1.0),F0);

        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * max(dot(N,V),0.0) * max(dot(N,L),0.0);
        vec3 specular = nominator / max(denominator,0.001);

        vec3 KS = F;
        vec3 KD = vec3(1.0) - KS;

        KD *= 1.0 - metallic;

        float NdotL = max(dot(N,L),0.0);

        Lo += (KD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 c = ambient + Lo;
    c = c / (c + vec3(1.0));
    c = pow(c,vec3(1.0/2.2));
    color = vec4(c,1.0);

    // color =vec4(uv,0.0,1.0);
    // color =vec4(normal,1.0);
}