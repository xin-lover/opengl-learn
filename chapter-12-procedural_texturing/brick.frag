#version 330 core

in vec2 texcoord;
in vec2 mcPos;
out vec4 color;

uniform vec3 brickColor,mortarColor;
uniform vec2 brickSize;
uniform vec2 brickPct;

void main()
{
	vec2 pos, useBrick;
	pos = mcPos / brickSize;

	if(fract(pos.y * 0.5) > 0.5)
	{
		pos.x += 0.5 ;
	}

	pos = fract(pos);
	useBrick = step(pos,brickPct);

	vec3 c = mix(mortarColor,brickColor,useBrick.x * useBrick.y);
	color = vec4(c,1.0);
}
