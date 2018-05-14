#version 330 core 

in vec2 texcoords;
uniform sampler2D texture_diffuse1;
out vec4 color;

void main()
{
	vec2 tc = vec2(texcoords.x,1- texcoords.y);
	vec4 c = texture(texture_diffuse1,tc);
	if(c.a < 0.1)
		discard;
	
	color = c;
	color = vec4(0,1,0,1);
}
