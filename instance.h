#pragma once
#include"hittable.h"
#include<climits>
const float PI = 3.1415926535897;

class translate :public hittable
{
public:
	translate() {}
	translate(hittable* p, const vec3& displacement) :
		ptr(p), offset(displacement) {}
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const
	{
		ray moved_r(r.origin() - offset, r.direction());
		return ptr->hit(moved_r, tmin, tmax, rec);
	}
	virtual bool bounding_box(float t0, float t1, aabb& box)const
	{
		if (ptr->bounding_box(t0, t1, box))
		{
			box = aabb(box.min() + offset, box.max() + offset); 
			return true;
		}
		else return false;
	}
	hittable* ptr;
	vec3 offset;
};

class rotate_y :public hittable
{
public:
	rotate_y(hittable* p, float angle);
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const ;
	virtual bool bounding_box(float t0, float t1, aabb& box)const
	{
		box = bbox;
		return hasbox;
	}
	hittable* ptr;
	float sin_theta, cos_theta;
	bool hasbox;
	aabb bbox;
};

rotate_y::rotate_y(hittable* p, float angle) :ptr(p)
{
	float radians = angle * PI / 180.0;
	sin_theta = sin(radians);
	cos_theta = cos(radians);
	hasbox = ptr->bounding_box(0, 1, bbox);
	vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
	vec3 max(-FLT_MIN, -FLT_MIN, -FLT_MIN);
	for(int i=0;i<2;i++)
		for(int j=0;j<2;j++)
			for (int k = 0; k < 2; k++)
			{
				float x = i * bbox.max().x() + (1 - i) * bbox.min().x();
				float y = j * bbox.max().y() + (1 - j) * bbox.min().y();
				float z = k * bbox.max().z() + (1 - j) * bbox.min().z();
				float newx = x * cos_theta + z * sin_theta;
				float newz = -x * sin_theta + z * cos_theta;
				vec3 tester(newx, y, newz);
				for (int c = 0; c < 3; c++)
				{
					if (tester[c] > max[c])
						max[c] = tester[c];
					if (tester[c] < min[c])
						min[c] = tester[c];
				}
			}
	bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	vec3 origin = r.origin();
	vec3 direction = r.direction();
	origin[0] = r.origin().x() * cos_theta - r.origin().z() * sin_theta;
	origin[2] = r.origin().x() * sin_theta + r.origin().z() * cos_theta;
	direction[0] = r.direction().x() * cos_theta - r.direction().z() * sin_theta;
	direction[2] = r.direction().x() * sin_theta + r.direction().z() * cos_theta;
	ray rotated_r(origin, direction);
	if (ptr->hit(rotated_r, tmin, tmax, rec))
	{
		vec3 p = rec.p;
		vec3 normal = rec.normal;
		p[0] = rec.p.x() * cos_theta + rec.p.z() * sin_theta;
		p[2] = -rec.p.x() * sin_theta + rec.p.z() * cos_theta;
		normal[0] = rec.normal.x() * cos_theta + rec.normal.z() * sin_theta;
		normal[2] = -rec.normal.x() * sin_theta + rec.normal.z() * cos_theta;
		rec.p = p;
		rec.normal = normal;
		return true;
	}
	else return false;
}
