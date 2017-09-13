#version 410 core

layout (location = 0) in vec3 pos;

uniform mat4		mat_total;

void main (void)
{
	gl_Position = mat_total * vec4(pos.x, pos.y, pos.z, 1.0f);
}
