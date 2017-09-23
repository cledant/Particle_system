__kernel void lifetime(__global t_particle *particle, float2 minmax,
	float2 lifetime , uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center, float tick_rate)
{
	int i = get_global_id(0);

	particle[i].lifetime.x -= tick_rate;
	if (particle[i].lifetime.x <= 0.0f)
	{
		particle[i].pos.x = linearRandom(minmax.x, minmax.y, randoms_x) + center.x;
		particle[i].pos.y = linearRandom(minmax.x, minmax.y, randoms_y) + center.y;
		particle[i].pos.z = linearRandom(minmax.x, minmax.y, randoms_z) + center.z;
		particle[i].pos.w = 1.0f;
		particle[i].vel.xyzw = 0.0f;
		particle[i].acc.xyzw = 0.0f;
		particle[i].lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_y);
	}
}
