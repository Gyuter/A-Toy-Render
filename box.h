#pragma once
#include"hittable.h"
#include"hittable_list.h"
#include"rectangle.h"
#include"flip_normals.h"

class box :public hittable
{
public:
	box() {}
	box(const vec3&, const vec3&, material*);
	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)
		const ;
	virtual bool bounding_box(float t0, float t1, aabb& box)const
	{
		box = aabb(pmin, pmax);
		return true;
	}

	vec3 pmin, pmax;
	hittable* list_ptr;
};

box::box(const vec3& p0, const vec3& p1, material* matp) :
	pmin(p0), pmax(p1)
{
	int i = 0;
	hittable** list = new hittable * [6];
	list[i++] = new flip_normals(new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), matp));
	list[i++] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), matp);
	list[i++] = new flip_normals(new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), matp));
	list[i++] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), matp);
	list[i++] = new flip_normals(new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), matp));
	list[i++] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), matp);
	list_ptr = new hittable_list(list, i);
}

bool box::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	return list_ptr->hit(r, tmin, tmax, rec);
}