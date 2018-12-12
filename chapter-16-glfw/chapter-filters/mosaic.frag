#version 330 core

uniform sampler2D img1;

in vec2 uv;

out vec4 fragColor;

const vec2 kTexSize = vec2(400.0,400.0);
const vec2 kMosaicSize = vec2(8.0,8.0);

void main()
{
	vec2 int_xy = vec2(uv.x * kTexSize.x,uv.y * kTexSize.y);
	vec2 mosaic_xy = vec2(floor(int_xy.x / kMosaicSize.x)*kMosaicSize.x,floor(int_xy.y / kMosaicSize.y) * kMosaicSize.y);
	vec2 mosaic_uv = vec2(mosaic_xy.x / kTexSize.x,mosaic_xy.y / kTexSize.y);
	fragColor = texture(img1,mosaic_uv);
}
