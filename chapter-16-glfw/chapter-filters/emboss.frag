#version 330 core

uniform sampler2D img1;

in vec2 uv;

out vec4 fragColor;

const vec3 W = vec3(0.2125,0.7154,0.0721);
const vec2 kTexSize =vec2(100.0,100.0);
const vec4 bkColor = vec4(0.5,0.5,0.5,1.0);

void main()
{
	vec2 upLeftUV = vec2(uv.x - 1.0 / kTexSize.x,uv.y - 1.0 / kTexSize.y);
	vec4 curColor = texture(img1,uv);
	vec4 upLeftColor = texture(img1,upLeftUV);
	vec4 delColor = curColor - upLeftColor;
	float luminance = dot(delColor.rgb,W);
	fragColor = vec4(vec3(luminance),0.0) + bkColor;
}
