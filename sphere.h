#pragma once
#include"hittable.h"
const float PI = 3.1415926535897;

class sphere :public hittable
{
public:
	sphere(){}
	sphere(vec3 cen,float r,material* matp):center(cen),radius(r),mat_ptr(matp){}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec)const;
	virtual bool bounding_box(float t0, float t1, aabb& box)const;

	vec3 center;
	float radius;
	material* mat_ptr;
};

void get_sphere_uv(const vec3& p, float& u, float& v)
{
	float phi = atan2(p.z(), p.x());
	float theta = asin(p.y());
	u = 1.0 - (phi + PI) / (2 * PI);
	v = (theta + PI / 2) / PI;
}

bool sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float tmp = (-b-sqrt(discriminant)) / a;
		if (tmp >= tmin && tmp <= tmax)
		{
			rec.p = r.point_at_parameter(tmp);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.t = tmp;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		tmp = (-b+sqrt(discriminant)) / a;
		if (tmp >= tmin && tmp <= tmax)
		{
			rec.p = r.point_at_parameter(tmp);
			get_sphere_uv((rec.p - center) / radius, rec.u, rec.v);
			rec.normal = (rec.p - center) / radius;
			rec.t = tmp;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
bool sphere::bounding_box(float t0, float t1, aabb& box)const
{
	box = aabb(
		center - vec3(radius, radius, radius),
		center + vec3(radius, radius, radius)
	);
	return true;
}

class moving_sphere :public hittable
{
public: 
	moving_sphere() {}
	moving_sphere(const vec3& cen0, const vec3& cen1,
		float t0, float t1, float r, material* m) :
		center0(cen0), center1(cen1), time0(t0), time1(t1),
		radius(r), mat_ptr(m) {}

	virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
	virtual bool bounding_box(float t0, float t1, aabb& box) const;
	vec3 center(float time) const;

	vec3 center0, center1;
	float time0, time1;
	float radius;
	material* mat_ptr;
};

vec3 moving_sphere::center(float time)const
{
	return (time - time0) / (time1 - time0) * (center1 - center0) + center0;
}

bool moving_sphere::bounding_box(float t0, float t1, aabb& box)const
{
	aabb start_box = aabb(
		center0 - vec3(radius, radius, radius),
		center0 + vec3(radius, radius, radius)
	);
	aabb end_box = aabb(
		center1 - vec3(radius, radius, radius),
		center1 + vec3(radius, radius, radius)
	);
	box = surrounding_box(start_box, end_box);
	return true;
}

bool moving_sphere::hit(const ray& r, float tmin, float tmax, hit_record& rec)const
{
	vec3 oc = r.origin() - center(r.time());
	float a = dot(r.direction(), r.direction());
	float b = dot(r.direction(), oc);
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	if (discriminant > 0)
	{
		float tmp = (-b - sqrt(discriminant)) / a;
		if (tmp >= tmin && tmp <= tmax)
		{
			rec.p = r.point_at_parameter(tmp);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.t = tmp;
			rec.mat_ptr = mat_ptr;
			return true;
		}
		tmp = (-b + sqrt(discriminant)) / a;
		if (tmp >= tmin && tmp <= tmax)
		{
			rec.p = r.point_at_parameter(tmp);
			rec.normal = (rec.p - center(r.time())) / radius;
			rec.t = tmp;
			rec.mat_ptr = mat_ptr;
			return true;
		}
	}
	return false;
}
