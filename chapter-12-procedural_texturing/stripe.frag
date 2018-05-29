#version 330 core

uniform vec3 stripeColor;
uniform vec3 backColor;
uniform float width;
uniform float scale;
uniform float fuzz;

in float texcoord;
out vec4 color;

void main()
{
	float scaledT = fract(texcoord * scale);
	float frac1 = clamp(scaledT / fuzz, 0.0,1.0);
	float frac2 = clamp((scaledT - width) / fuzz, 0.0, 1.0);

	frac1 = frac1 * (1.0 - frac2);
	frac1 = frac1 * frac1 * (3.0 - (2.0 * frac1));

	vec3 finalColor = mix(backColor, stripeColor, frac1);
	color = vec4(finalColor,1.0);
}
