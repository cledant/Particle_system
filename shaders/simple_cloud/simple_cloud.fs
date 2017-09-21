#version 410 core

uniform vec3		cloud_color;
uniform vec3		mouse_3d_pos;

in vec3				pos_fs;

out vec4			color;

void main (void)
{
	vec3	final_color;

	final_color = cloud_color + abs(normalize(mouse_3d_pos - pos_fs));
	final_color.x = min(final_color.x, 1.0f);
	final_color.y = min(final_color.y, 1.0f);
	final_color.z = min(final_color.z, 1.0f);
	color = vec4(final_color, 1.0f);
}
