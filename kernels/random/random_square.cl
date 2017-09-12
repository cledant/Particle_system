uint		random_number_generator(uint2 randoms);
float		linearRandom(float min, float max, uint2 randoms);

uint		random_number_generator(uint2 randoms)
{
	uint i = get_global_id(0);
	uint seed = randoms.x + i;
	uint t = seed ^ (seed << 11);

	return (randoms.y ^ (randoms.y >> 19) ^ (t ^ (t >> 8)));
}

float		linearRandom(float min, float max, uint2 randoms)
{
	return ((random_number_generator(randoms) / 4294967295.0f) * (max - min) + min);
}

kernel void random_square(__global float3 *particle, float min, float max, 
	int useless, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z)
{
	int i = get_global_id(0);

	particle[i].x = linearRandom(min, max, randoms_x);
	particle[i].y = linearRandom(min, max, randoms_y);
	particle[i].z = linearRandom(min, max, randoms_z);
}
