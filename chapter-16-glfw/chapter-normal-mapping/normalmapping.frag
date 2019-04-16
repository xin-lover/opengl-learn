#version 330 core

uniform sampler2D wall;
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

uniform sampler2D normalmap;

void main()
{  
    // color = vec4(1.0,0.0,0.0,1.0);

    vec3 normal = texture(normalmap,fs_in.uv).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    float ambient = 0.1;
    // vec3 fragToLight = normalize(lightPos - fragPos);
    // float diffuse = max(dot(normalize(fragToLight), normal),0.0);

    // vec3 fragToEye = normalize(viewPos - fragPos.xyz);
    // vec3 reflectLight = reflect(-fragToLight,normal);
    // float spec = max(dot(reflectLight, fragToEye),0.0);
    // spec = pow(spec,32) * 0.8;

    vec3 fragToLight = normalize(fs_in.tangentLightPos - fs_in.tangentFragPos);
    float diffuse = max(dot(fragToLight,normal),0.0);

    vec3 fragToEye = normalize(fs_in.tangentViewPos - fs_in.tangentFragPos);
    // vec3 reflectLight = reflect(-fragToLight,normal);
    vec3 halfwayDir = normalize(fragToLight+fragToEye);
    float spec = pow(max(dot(normal,halfwayDir),0.0),32.0);

    vec3 objColor = texture(wall,fs_in.uv).rgb;
    vec3 res = (ambient* objColor + diffuse * objColor + spec * vec3(0.2)) * lightColor;
    color = vec4(res,1.0);
}