uint		random_number_generator(uint2 randoms);
float		linearRandom(float min, float max, uint2 randoms);

uint		random_number_generator(uint2 randoms)
{
	uint i = get_global_id(0);
	uint seed = (randoms.x + i * 168468498) ^ randoms.y;
	seed = (seed << 9) * seed;
	uint t = seed ^ (seed << 11);
	t = (t >> 5) ^ (t * 9135723);
	uint result = randoms.y ^ (randoms.y >> 19) ^ (t ^ (t >> 8));
	result = (result << 7) * 415645;

	return (result);
}

float		linearRandom(float min, float max, uint2 randoms)
{
	float ran = (random_number_generator(randoms) / 4294967295.0f);
	float run = -ran;
	float rin;	

	ran = cos(50 * ran);	
	run = sin(50 * run);
	rin = tan(ran * run * 30);

	return (rin * (max - min) + min);
}

__kernel void random_square(__global float3 *particle, float min, float max, 
	int useless, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z)
{
	int i = get_global_id(0);

	particle[i].x = linearRandom(min, max, randoms_y);
	particle[i].y = linearRandom(min, max, randoms_x);
	particle[i].z = linearRandom(min, max, randoms_z);
}
