#version 330 core

uniform sampler3D noise;
uniform vec3 skyColor;
uniform vec3 cloudColor;

in float lightIntensity;
in vec3 mcPos;

out vec4 color;

void main()
{
	vec4 noisevec = texture(noise,mcPos);
	float intensity = (noisevec[0] + noisevec[1] + noisevec[2] + noisevec[3] + 0.03125) * 1.5;

	vec3 c = mix(skyColor,cloudColor,intensity) * lightIntensity;
	color = vec4(c,1.0);
	//color = noisevec;
}
