float3		calculate_gravity(float3 particle_pos, float part_mass,
				float center_mass, float3 pos_gravity)
{
	float		gravity_cte = 	0.00674;
	float3		vec_dist;
	float		dist;

	 if ((dist = distance(particle_pos, pos_gravity)) < 0.001f)
		dist = 0.001f;
	vec_dist = normalize(particle_pos - pos_gravity);
	return (((gravity_cte * part_mass * center_mass) / (dist * dist)) * vec_dist);
}

__kernel void gravity(__global t_particle *part, float3 pos_gravity,
				float delta_time, float part_mass, float center_mass)
{
	float3		new_acc;

	part->acc = calculate_gravity(part->pos, part_mass, center_mass, pos_gravity);
	part->pos = delta_time * (part->vel + delta_time * part->acc);
	new_acc = calculate_gravity(part->pos, part_mass, center_mass, pos_gravity);
	part->vel = delta_time * ((part->acc + new_acc) / 2);
}
