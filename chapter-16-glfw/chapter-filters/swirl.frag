#version 330 core
out vec4 fragColor;

const float PI = 3.1415926;
const float uD = 80.0;
const float uR = 0.5;

in vec2 uv;
uniform sampler2D img1;

void main()
{
	vec2 dxy = (uv - vec2(0.5,0.5));
	float r = length(dxy);
	float beta = atan(dxy.y,dxy.x) + radians(uD) * 2.0 * (1 - (r / 0.5) * (r / 0.5));

	vec2 xy1 = uv;
	if(r <= 0.5)
	{
		xy1 = 0.5 + r * vec2(cos(beta),sin(beta));
	}

	fragColor = texture(img1,xy1);
}
