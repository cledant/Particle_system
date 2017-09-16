float4		calculate_gravity(float4 particle_pos, float part_mass,
				float center_mass, float4 pos_gravity)
{
	float		gravity_cte = 	0.0674;
	float4		vec_dist;
	float		dist;

	 if ((dist = distance(particle_pos, pos_gravity)) < 0.00001f)
		dist = 0.00001f;
	vec_dist = normalize(particle_pos - pos_gravity);
	return (((gravity_cte * part_mass * center_mass) / (dist * dist)) * vec_dist);
}

__kernel void gravity(__global t_particle *part, float4 pos_gravity,
				float delta_time, float part_mass, float center_mass)
{
	float4		new_acc;
	int			i = get_global_id(0);


	part[i].acc = calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].pos = delta_time * (part[i].vel + delta_time * part[i].acc);
	new_acc = calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].vel = delta_time * ((part[i].acc + new_acc) / 2);
}
