__kernel void random_sphere(__global t_particle *particle, float2 minmax,
	float2 lifetime, uint2 randoms_x, uint2 randoms_y, uint2 randoms_z,
	float3 center)
{
	int i = get_global_id(0);

	float z = linearRandom(-1.0f, 1.0f, randoms_z);
	float angle = linearRandom(0.0f, 6.29f, randoms_x);
	float radius = sqrt((1.0f - z) * z);
	minmax.y *= 2;

	particle[i].pos.x = (radius * cos(angle)) * minmax.y + center.x;
	particle[i].pos.y = (radius * sin(angle)) * minmax.y + center.y;
	particle[i].pos.z = z * minmax.y + center.z - (minmax.y / 2);
	particle[i].pos.w = 1.0f;
	particle[i].vel.xyzw = 0.0f;
	particle[i].acc.xyzw = 0.0f;
	particle[i].lifetime.x = linearRandom(lifetime.x, lifetime.y, randoms_y);
}
