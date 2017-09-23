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

__kernel void random_cross(__global t_particle *particle, float min, float max, 
	uint2 randoms_x, uint2 randoms_y, uint2 randoms_z, float4 center)
{
	int i = get_global_id(0);

	particle[i].pos.x = linearRandom_cross(min, max, randoms_y) + center.x;
	particle[i].pos.y = linearRandom_cross(min, max, randoms_x) + center.y;
	particle[i].pos.z = linearRandom_cross(min, max, randoms_z) + center.z;
	particle[i].pos.w = 1.0f;
	particle[i].vel.x = 1.0f;
	particle[i].vel.y = 1.0f;
	particle[i].vel.z = 1.0f;
	particle[i].vel.w = 1.0f;
	particle[i].acc.x = 1.0f;
	particle[i].acc.y = 1.0f;
	particle[i].acc.z = 1.0f;
	particle[i].acc.w = 1.0f;
	particle[i].lifetime.x = 200.0f;
}
