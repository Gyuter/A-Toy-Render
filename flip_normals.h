#pragma once

#include"hittable.h"

class flip_normals :public hittable
{
public:
	flip_normals(hittable* p) :ptr(p) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const
	{
		if (ptr->hit(r, tmin, tmax, rec))
		{
			rec.normal = -rec.normal;
			return true;
		}
		else return false;
	}
	virtual bool bounding_box(float t0, float t1, aabb& box)const
	{
		return ptr->bounding_box(t0, t1, box);
	}

	hittable* ptr;
};