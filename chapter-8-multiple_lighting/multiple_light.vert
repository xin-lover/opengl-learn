#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

layout(location=0) in vec3 in_pos;
layout(location=1) in vec3 in_norm;
layout(location=2) in vec2 in_uv;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main()
{
	normal = normalize(mat3(model) * in_norm);
	fragPos = vec3(model * vec4(in_pos,1.0));
	texCoords = in_uv;
	gl_Position = proj * view * model * vec4(in_pos,1.0);
}
