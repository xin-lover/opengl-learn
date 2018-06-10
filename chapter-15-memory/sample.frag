#version 430 core

layout(binding=0,rgba32f) uniform image2D colors;
layout(binding=1,rgba32f) uniform image2D output_buffer;

//uniform sampler2D image;

in vec2 texcoords;
out vec4 color;

void main()
{
	//ivec2 pos = ivec2(gl_GlobalInvocationID.xy);
	ivec2 size = imageSize(output_buffer);
	vec4 col = imageLoad(colors,ivec2(1,0));//ivec2(gl_FragCoord.xy));
	imageStore(output_buffer,ivec2(size.x * texcoords.x, size.y * texcoords.y),vec4(texcoords,0,1));
	//imageStore(output_buffer,ivec2(texcoords),vec4(1,1,0,1));
	color = col;//vec4(1,0,0,1);
	//color = texture(image,texcoords);
}
