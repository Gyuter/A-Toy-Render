#pragma once
#include"hittable.h"

class hittable_list :public hittable
{
public:
	hittable_list () {}
	hittable_list(hittable** l, int n) :list(l), list_size(n) {}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box)const;

	hittable** list;
	int list_size;
};

bool hittable_list::bounding_box(float t0, float t1, aabb& box)const
{
	if (list_size < 1) return false;
	aabb temp_box;
	bool first_true = list[0]->bounding_box(t0, t1, temp_box);
	if (!first_true) return false;
	box = temp_box;
	for (int i = 1; i < list_size; i++)
		if (list[i]->bounding_box(t0, t1, temp_box))
			box = surrounding_box(temp_box, box);
		else return false;
	return true;
}


bool hittable_list::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	hit_record tmp_rec;
	bool hit_anything = false;
	float closest_so_far = tmax;
	for(int i=0;i<list_size;i++)
		if (list[i]->hit(r, tmin, closest_so_far, tmp_rec))
		{
			hit_anything = true;
			closest_so_far = tmp_rec.t;
			rec = tmp_rec;
		}
	return hit_anything;
}
