float		linearRandom_cute(float min, float max, uint2 randoms)
{
	uint nb = random_number_generator(randoms);
	uint nb_2 = random_number_generator(nb);
	float ran = (nb / 4294967295.0f);
	float run = -ran;
	float rin = (nb_2 / 4294967295.0f);
	float ren = -rin;
	float ron;

	ran = tan(40 * ran);	
	run = sin(40 * run);
	rin = tan(-30 * rin);
	ren = cos(-30 * ren);

	ron = ran * rin * ren * run;

	return (ron * (max - min) + min);
}

__kernel void random_cute(__global t_particle *particle, float2 minmax,
	float2 lifetime, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom_cute(minmax.x, minmax.y, randoms_y);
	particle[i].pos.y = linearRandom_cute(minmax.x, minmax.y, randoms_x);
	particle[i].pos.z = linearRandom_cute(minmax.x, minmax.y, randoms_z);
	particle[i].pos.w = 1.0f;
	particle[i].vel.xyzw = 0.0f;
	particle[i].acc.xyzw = 0.0f;
	particle[i].lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_y);
}
