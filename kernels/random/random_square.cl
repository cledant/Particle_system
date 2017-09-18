uint		random_number_generator(uint2 randoms)
{
	uint i = get_global_id(0);
	uint seed = (randoms.x + i * 168468498) ^ (randoms.y << 8);
	seed = (seed << 9) * seed;
	uint t = seed ^ (seed << 11);
	t = (t >> 5) ^ (t * 9135723);
	uint result = randoms.y ^ (randoms.y >> 19) ^ (t ^ (t >> 8));
	result = (result << 7) * 415645;

	return (result);
}

float		linearRandom(float min, float max, uint2 randoms)
{
	uint nb = random_number_generator(randoms);
	float ran = (nb / 4294967295.0f);

	return (ran * (max - min) + min);
}

__kernel void random_square(__global t_particle *particle, float min, float max, 
	uint2 randoms_x, uint2 randoms_y, uint2 randoms_z, float4 pos_gravity,
	float open_cl_cte)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom(min, max, randoms_x);
	particle[i].pos.y = linearRandom(min, max, randoms_y);
	particle[i].pos.z = linearRandom(min, max, randoms_z);
	particle[i].pos.w = 100.0f;
	particle[i].vel.x = 0.0f;
	particle[i].vel.y = 0.0f;
	particle[i].vel.z = 0.0f;
	particle[i].vel.w = 0.0f;
	particle[i].new_acc = calculate_acceleration(particle[i].pos, pos_gravity,
		open_cl_cte);
}
