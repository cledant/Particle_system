#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4		mat_total;
out	vec3			pos_fs;

void main (void)
{
	pos_fs = pos;
	gl_Position = mat_total * vec4(pos.x, pos.y, pos.z, 1.0f);
}
