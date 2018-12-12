#version 330 core
out vec4 fragColor;

in vec2 uv;
uniform sampler2D img1;
uniform sampler2D img2;

void main()
{
	fragColor = vec4(0,1,0,1.0);
	vec4 icolor = texture(img2,uv);
	fragColor = mix(fragColor,icolor,icolor.a);
	if(fragColor.a < 0.1)
	{

	//	fragColor = vec4(uv,0,1.0);
	}
}
