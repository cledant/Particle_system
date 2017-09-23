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

__kernel void random_cute(__global t_particle *particle, float min, float max, 
	int useless, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom_cute(min, max, randoms_y);
	particle[i].pos.y = linearRandom_cute(min, max, randoms_x);
	particle[i].pos.z = linearRandom_cute(min, max, randoms_z);
	particle[i].pos.w = 1.0f;
	particle[i].vel.x = 0.0f;
	particle[i].vel.y = 0.0f;
	particle[i].vel.z = 0.0f;
	particle[i].vel.w = 0.0f;
	particle[i].acc.x = 0.0f;
	particle[i].acc.y = 0.0f;
	particle[i].acc.z = 0.0f;
	particle[i].acc.w = 0.0f;
	particle[i].lifetime.x = 200.0f;
}
