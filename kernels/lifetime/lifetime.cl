void 	reset_particle(__global t_particle *particle, float2 minmax,
	float2 lifetime, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center, float tick_rate)
{
	particle->pos.x = linearRandom(minmax.x, minmax.y, randoms_x) + center.x;
	particle->pos.y = linearRandom(minmax.x, minmax.y, randoms_y) + center.y;
	particle->pos.z = linearRandom(minmax.x, minmax.y, randoms_z) + center.z;
	particle->pos.w = 1.0f;
	particle->vel.xyzw = 0.0f;
	particle->acc.xyzw = 0.0f;
	particle->lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_y);
}
