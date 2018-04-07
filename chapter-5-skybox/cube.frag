
#version 330 core

in vec2 vs_uv;
out vec4 color;

uniform sampler2D tex1;

void main()
{
	color = texture(tex1,vs_uv);
	//color = vec4(vec3(gl_FragCoord.z),1.0);
}
