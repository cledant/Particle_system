float4		calculate_acceleration(float4 particle_pos, float part_mass,
				float center_mass, float4 pos_gravity)
{
	float		gravity_cte = 0.000000000674;
	float4		vec_dist;
	float		dist;

	dist = fabs(distance(particle_pos, pos_gravity) + 6400000.0f);
	vec_dist = normalize(pos_gravity - particle_pos);
	return (((gravity_cte * part_mass * center_mass) *
				native_recip(powr(dist, 2)) * vec_dist));
}

__kernel void gravity(__global t_particle *part, float3 pos_gravity,
				float4 params, uint2 randoms_x, uint2 randoms_y,
				uint2 randoms_z, float2 minmax, float2 lifetime,
				float3 emitter_pos, char update_lifetime)
{
	float4		new_acc;
	float4		pos_grav;
	int			i = get_global_id(0);

	pos_grav.xyz = pos_gravity.xyz;
	pos_grav.w = 1.0f;
	if (update_lifetime == true)
		part[i].lifetime.x -= (params.x * 10.0f);
	if (part[i].lifetime.x <= 0.0f)
	{
		reset_particle(&(part[i]), minmax, lifetime, randoms_x, randoms_y,
			randoms_z, emitter_pos, params.x * 10.0f);
	}
	else
	{
		part[i].acc = params.w * calculate_acceleration(part[i].pos, params.y,
			params.z, pos_grav);
		part[i].pos += params.x * (part[i].vel + params.x * part[i].acc);
		new_acc = params.w * calculate_acceleration(part[i].pos, params.y, params.z,
			pos_grav);
		part[i].vel += params.x * ((part[i].acc + new_acc) / 2);
		part[i].vel = fmin(part[i].vel, MAX_SPEED);
		part[i].vel = fmax(part[i].vel, -MAX_SPEED);
	}
}
