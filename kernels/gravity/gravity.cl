float4		calculate_gravity(float4 particle_pos, float part_mass,
				float center_mass, float4 pos_gravity)
{
	float		gravity_cte = 0.000000000674;
	float4		vec_dist;
	float		dist;
	float		val;

	 if ((dist = distance(particle_pos, pos_gravity)) < 0.00001f)
		dist = 0.00001f;
//	printf("====================\n");
//	printf("dist = %f\n", dist);
	vec_dist = normalize(particle_pos - pos_gravity);
//	printf("vec dist x y z = %f | %f | %f\n", vec_dist.x, vec_dist.y, vec_dist.z);

//	printf("part mass= %f\n", part_mass);
//	printf("center mass = %f\n", center_mass);
//	printf("grav cte = %f\n", gravity_cte);

	val = ((gravity_cte * part_mass * center_mass) / (dist * dist));
//	printf("val = %f\n", val);
//	printf("====================\n");
	return (((gravity_cte * part_mass * center_mass) / (dist * dist)) * vec_dist);
}

__kernel void gravity(__global t_particle *part, float4 pos_gravity,
				float delta_time, float part_mass, float center_mass)
{
	float4		new_acc;
	int			i = get_global_id(0);

	if (delta_time <= 0.001f)
		delta_time = 0.001f;
//	printf("=====> delta_time = %f\n", delta_time);
	part[i].acc = calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].pos = delta_time * (part[i].vel + delta_time * part[i].acc);
	new_acc = calculate_gravity(part[i].pos, part_mass, center_mass, pos_gravity);
	part[i].vel = delta_time * ((part[i].acc + new_acc) / 2);
//	printf("pos x y z = %f | %f | %f\n", part[i].pos.x,  part[i].pos.y, part[i].pos.z);
}
