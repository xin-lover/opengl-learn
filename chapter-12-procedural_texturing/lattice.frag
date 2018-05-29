#version 330 core

in vec2 texcoord;
out vec4 color;

uniform vec2 scale;
uniform vec2 threshold;

void main()
{
	float ss = fract(texcoord.s * scale.s);
	float tt = fract(texcoord.t * scale.t);

	if((ss > threshold.s) && (tt > threshold.t))
		discard;

	color = vec4(1,0,0,1);
}
