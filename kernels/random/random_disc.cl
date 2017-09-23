__kernel void random_disc(__global t_particle *particle, float min, float max, 
	uint2 randoms_x, uint2 randoms_y, uint2 randoms_z, float4 center)
{
	int i = get_global_id(0);

	float z = linearRandom(-1.0f, 1.0f, randoms_z);
	float angle = linearRandom(0.0f, 6.28f, randoms_x);
	float radius = sqrt((1.0f - z) * z);
	max = max * 2;

	particle[i].pos.x = (radius * cos(angle)) * max + center.x;
	particle[i].pos.y = (radius * sin(angle)) * max + center.y;
	particle[i].pos.z = 0.0f + center.z;
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
