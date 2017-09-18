float4		calculate_acceleration(float4 particle_pos, float4 pos_gravity,
				float open_cl_cte)
{
	float4		vec_dist;
	float		dist;

	dist = distance(particle_pos, pos_gravity) + 6400000.0f;
	vec_dist = normalize(pos_gravity - particle_pos);
	return ((open_cl_cte / (dist * dist)) * vec_dist);
}

__kernel void gravity(__global t_particle *part, float4 pos_gravity,
				float delta_time, float open_cl_cte)
{
	int			i = get_global_id(0);
	float		lifetime;

	lifetime = part[i].pos.w;
	part[i].pos += delta_time * (part[i].vel + part[i].new_acc);
	part[i].old_acc = part[i].new_acc;
	part[i].new_acc = calculate_acceleration(part[i].pos, pos_gravity, open_cl_cte);
	part[i].vel += (part[i].old_acc + part[i].new_acc) / 2;
	part[i].pos.w = lifetime;
}
