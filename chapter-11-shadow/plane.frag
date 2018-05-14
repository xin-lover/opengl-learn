#version 330 core 

in VS_OUT {
	vec3 fragPos;
	vec4 fragPosLightSpace;
} fs_in;

in vec2 texcoords;
uniform sampler2D shadowMap;
out vec4 color;

float ShadowCalculation(vec4 fragPosLightSpace)
{
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = currentDepth > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	vec4 fragPosLightSpace = fs_in.fragPosLightSpace;
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;

	float shadow = ShadowCalculation(fs_in.fragPosLightSpace);
	vec3 red = vec3(1,0,0);
	vec3 lighting = vec3(0.1,0.1,0.1) +  (1-shadow) * red;
	color =  vec4(lighting,1.0);
	//color = vec4(vec3(texture(shadowMap,vec2(texcoords.x, texcoords.y)).r),1.0);
	//color = vec4(fs_in.fragPosLightSpace.xy,0,1);
	//color = vec4(fs_in.fragPosLightSpace.xyzw);
	//color = vec4(vec3(texture(shadowMap,projCoords.xy).r),1);
	//color = vec4(vec3(projCoords.y),1);
	//color = vec4(texcoords,0,1);
	//color = vec4(vec3(texture(shadowMap,texcoords).r),1.0);
	//color = vec4(1,0,0,1);
}
