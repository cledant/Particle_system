float		linearRandom_cross(float min, float max, uint2 randoms)
{
	float ran = (random_number_generator(randoms) / 4294967295.0f);
	float run = -ran;
	float rin;	

	ran = cos(50 * ran);	
	run = sin(50 * run);
	rin = tan(ran * run * 30);

	return (rin * (max - min) + min);
}

__kernel void random_cross(__global t_particle *particle, float2 minmax,
	float2 lifetime, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom_cross(minmax.x, minmax.y, randoms_y)
		+ center.x;
	particle[i].pos.y = linearRandom_cross(minmax.x, minmax.y, randoms_x)
		+ center.y;
	particle[i].pos.z = linearRandom_cross(minmax.x, minmax.y, randoms_z)
		+ center.z;
	particle[i].pos.w = 1.0f;
	particle[i].vel.xyzw = 0.0f;
	particle[i].acc.xyzw = 0.0f;
	particle[i].lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_y);
}
