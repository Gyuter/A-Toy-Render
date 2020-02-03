#pragma once
#include"ray.h"
#include<algorithm>

#bazingas here

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb
{
public:
	aabb() = default;
	aabb(const vec3& a, const vec3& b) { _min = a; _max = b; }

	vec3 min() { return _min; }
	vec3 max() { return _max; }

	bool hit(const ray& r, float tmin, float tmax) const
	{
		for (int i = 0; i < 3; i++)
		{
			float invD = 1.0 / r.direction()[i];
			float t0 = (_min[i] - r.origin()[i]) * invD;
			float t1 = (_max[i] - r.origin()[i]) * invD;
			if (t0 > t1) std::swap(t0, t1);
			tmin = tmin < t0 ? t0 : tmin;
			tmax = tmax > t1 ? t1 : tmax;
			if (tmin > tmax) return false;
		}
		return true;
	}


	vec3 _min, _max;
};

aabb surrounding_box(aabb& box0,aabb box1)
{
	vec3 _min = vec3(
		ffmin(box0.min().x(), box1.min().x()),
		ffmin(box0.min().y(), box1.min().y()),
		ffmin(box0.min().z(), box1.min().z())
	);
	vec3 _max = vec3(
		ffmax(box0.max().x(), box1.max().x()),
		ffmax(box0.max().y(), box1.max().y()),
		ffmax(box0.max().z(), box1.max().z())
	);
	return aabb(_min, _max);
}