#version 330 core

out float color;
in vec2 uv;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;

uniform vec3 samples[64];

int kernelSize = 64;
float radius = 0.5;
float bias = 0.5;

const vec2 noiseScale = vec2(1280.0/4.0,720.0/4.0);

uniform mat4 projection;

void main()
{
    vec3 fragPos = texture(gPosition,uv).rgb;
    vec3 normal = normalize(texture(gNormal,uv).rgb);
    vec3 randomVec = normalize(texture(texNoise,uv * noiseScale).rgb);

    vec3 tangent = normalize(randomVec-normal * dot(randomVec,normal));
    vec3 bitangent = cross(tangent,normal);
    mat3 TBN = mat3(tangent,bitangent,normal);
    float occlusion = 0.0;
    for(int i=0;i<kernelSize;++i)
    {
        vec3 sample = TBN * samples[i];
        sample = fragPos + sample * radius;

        vec4 offset = vec4(sample,1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sampleDepth = texture(gPosition,offset.xy).z;

        float rangeCheck = smoothstep(0.0,1.0,radius / abs(fragPos.z - sampleDepth));
        occlusion += (sampleDepth >= sample.z + bias ? 1.0 : 0.0) * rangeCheck;
    }

    occlusion = 1.0 - (occlusion / kernelSize);
    color = occlusion;
}