float4		calculate_gravity(float4 particle_pos, float part_mass,
				float center_mass, float4 pos_gravity)
{
	float		gravity_cte = 0.000000000674;
	float4		vec_dist;
	float		dist;

	dist = distance(particle_pos, pos_gravity) + 6400000.0f;
	vec_dist = normalize(pos_gravity - particle_pos);
	return (((gravity_cte * part_mass * center_mass) / (dist * dist)) * vec_dist);
}

__kernel void gravity(__global t_particle *part, float4 pos_gravity,
				float delta_time, float part_mass, float center_mass,
				float grav_mult)
{
	float4		new_acc;
	int			i = get_global_id(0);

	part[i].acc = grav_mult * calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].pos += delta_time * (part[i].vel + delta_time * part[i].acc);
	new_acc = grav_mult * calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].vel += delta_time * ((part[i].acc + new_acc) / 2);
	if (part[i].vel.x >= MAX_SPEED)
		part[i].vel.x = MAX_SPEED;
	if (part[i].vel.y >= MAX_SPEED)
		part[i].vel.y = MAX_SPEED;
	if (part[i].vel.z >= MAX_SPEED)
		part[i].vel.z = MAX_SPEED;
	if (part[i].vel.x <= -MAX_SPEED)
		part[i].vel.x = -MAX_SPEED;
	if (part[i].vel.y <= -MAX_SPEED)
		part[i].vel.y = -MAX_SPEED;
	if (part[i].vel.z <= -MAX_SPEED)
		part[i].vel.z = -MAX_SPEED;
}
