#version 330 core

uniform sampler2D img1;

in vec2 uv;

out vec4 fragColor;

const vec3 W = vec3(0.2125,0.7154,0.0721);
const vec2 kTexSize =vec2(100.0,100.0);
const vec4 bkColor = vec4(0.5,0.5,0.5,1.0);

void main()
{
	float dx = 1.0 / kTexSize.x;
	float dy = 1.0 / kTexSize.y;

	vec2 offsets[4];
	offsets[0] = vec2(-dx,0) * 2;
	offsets[1] = vec2(dx,0) * 2;
	offsets[2] = vec2(dy,0) * 2;
	offsets[3] = vec2(-dy,0) * 2;
	vec4 c = vec4(0.0,0.0,0.0,0.0);
	for(int i=0;i<4;++i)
	{
		vec2 tuv = uv + offsets[i];
		c = c + texture(img1,tuv);
	}
	c = c * 0.25;
	fragColor = c;
}
