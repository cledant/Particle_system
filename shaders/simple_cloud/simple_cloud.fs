#version 410 core

uniform vec3		cloud_color;
uniform vec3		mouse_3d_pos;

in vec3				pos_fs;

out vec4			color;

void main (void)
{
	vec3	final_color;
	vec3	toto;

	final_color = abs(normalize(mouse_3d_pos - pos_fs));
	toto = cloud_color;
	color = vec4(final_color, 1.0f);
}
