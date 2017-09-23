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

__kernel void random_square(__global t_particle *particle, float2 minmax,
	float2 lifetime, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom(minmax.x, minmax.y, randoms_x) + center.x;
	particle[i].pos.y = linearRandom(minmax.x, minmax.y, randoms_y) + center.y;
	particle[i].pos.z = linearRandom(minmax.x, minmax.y, randoms_z) + center.z;
	particle[i].pos.w = 1.0f;
	particle[i].vel.xyzw = 0.0f;
	particle[i].acc.xyzw = 0.0f;
	particle[i].lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_z);
}
