#version 330 core

out vec4 color;

in VS_OUT
{
    vec3 fragPos;
    vec2 uv;
    vec3 tangentLightPos;
    vec3 tangentViewPos;
    vec3 tangentFragPos;
}fs_in;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;

uniform sampler2D diffuseMap;
uniform sampler2D normalMap;
uniform sampler2D depthMap;

uniform float heightScale;

vec2 ParallaxMapping(vec2 texCoords,vec3 viewDir)
{
    float height = texture(depthMap,texCoords).r;
    return texCoords - viewDir.xy * (height * heightScale);
}

void main()
{  
    // color = vec4(1.0,0.0,0.0,1.0);
    vec3 fragToEye = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    vec2 uv2 = ParallaxMapping(fs_in.uv,fragToEye);
    if(uv2.x > 1.0 || uv2.x < 0.0 || uv2.y > 1.0 || uv2.y < 0.0)
        discard;

    vec3 normal = texture(normalMap,uv2).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 objColor = texture(diffuseMap,uv2).rgb;
    float ambient = 0.1;
    // vec3 fragToLight = normalize(lightPos - fragPos);
    // float diffuse = max(dot(normalize(fragToLight), normal),0.0);

    // vec3 fragToEye = normalize(viewPos - fragPos.xyz);
    // vec3 reflectLight = reflect(-fragToLight,normal);
    // float spec = max(dot(reflectLight, fragToEye),0.0);
    // spec = pow(spec,32) * 0.8;

    vec3 fragToLight = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    float diffuse = max(dot(fragToLight,normal),0.0);

    // vec3 reflectLight = reflect(-fragToLight,normal);
    vec3 halfwayDir = normalize(fragToLight+fragToEye);
    float spec = pow(max(dot(normal,halfwayDir),0.0),32.0);

    vec3 res = (ambient * objColor + diffuse * objColor + spec * vec3(0.2)) * lightColor;
    color = vec4(res,1.0);
}