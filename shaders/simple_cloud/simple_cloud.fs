#version 410 core

uniform vec3		cloud_color;

out vec4			color;

void main (void)
{
	color = vec4(cloud_color, 1.0f);
}
