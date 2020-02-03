#pragma once
#include<iostream>
#include<stdlib.h>
#include<math.h>

class vec3
{
public:
	vec3() = default;
	vec3(float e0, float e1, float e2)
	{
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const vec3& operator+() { return *this; }
	inline vec3 operator-() { return vec3(-e[0], -e[1], -e[2]); }

	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline vec3& operator+=(const vec3& v2)
	{
		e[0] += v2.e[0];
		e[1] += v2.e[1];
		e[2] += v2.e[2];
		return *this;
	}
	inline vec3& operator-= (const vec3& v2)
	{
		e[0] -= v2.e[0];
		e[1] -= v2.e[1];
		e[2] -= v2.e[2];
		return *this;
	}
	inline vec3& operator*= (const vec3& v2)
	{
		e[0] *= v2.e[0];
		e[1] *= v2.e[1];
		e[2] *= v2.e[2];
		return *this;
	}
	inline vec3& operator/=(const vec3& v2)
	{
		e[0] /= v2.e[0];
		e[1] /= v2.e[1];
		e[2] /= v2.e[2];
		return *this;
	}
	inline vec3& operator*=(const float k)
	{
		e[0] *= k;
		e[1] *= k;
		e[2] *= k;
		return *this;
	}
	inline vec3& operator/=(const float k)
	{
		e[0] /= k;
		e[1] /= k;
		e[2] /= k;
		return *this;
	}

	inline float length() const
	{
		return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
	}
	inline float squared_length() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}
	inline void make_unit_vector()
	{
		float len = length();
		e[0] /= len;
		e[1] /= len;
		e[2] /= len;
	}

	float e[3];
};

inline vec3 operator+(const vec3& a, const vec3& b)
{
	return vec3(a.e[0] + b.e[0], a.e[1] + b.e[1], a.e[2] + b.e[2]);
}
inline vec3 operator-(const vec3& a, const vec3& b)
{
	return vec3(a.e[0] - b.e[0], a.e[1] - b.e[1], a.e[2] - b.e[2]);
}
inline vec3 operator*(const vec3& a, const vec3& b)
{
	return vec3(a.e[0] * b.e[0], a.e[1] * b.e[1], a.e[2] * b.e[2]);
}
inline vec3 operator/(const vec3& a, const vec3& b)
{
	return vec3(a.e[0] / b.e[0], a.e[1] / b.e[1], a.e[2] / b.e[2]);
}

inline vec3 operator*(const float k,const vec3& a)
{
	return vec3(a.e[0] * k, a.e[1] * k, a.e[2] * k);
}
inline vec3 operator/(const float k,const vec3& a)
{
	return vec3(a.e[0] / k, a.e[1] / k, a.e[2] / k);
}
inline vec3 operator*(const vec3& a,const float k)
{
	return vec3(a.e[0] * k, a.e[1] * k, a.e[2] * k);
}
inline vec3 operator/(const vec3& a, const float k)
{
	return vec3(a.e[0] / k, a.e[1] / k, a.e[2] / k);
}

inline vec3 operator-(const vec3& a)
{
	return vec3(-a[0], -a[1], -a[2]);
}

//This realization may be dangerous and useless.
inline vec3& operator+(vec3& a)
{
	return a;
}

inline float dot(const vec3 a, const vec3 b)
{
	return a.e[0] * b.e[0] + a.e[1] * b.e[1] + a.e[2] * b.e[2];
}
inline vec3 cross(const vec3& a, const vec3& b)
{
	return vec3(a.e[1] * b.e[2] - a.e[2] * b.e[1],
				a.e[2] * b.e[0] - a.e[0] * b.e[2],
				a.e[0] * b.e[1] - a.e[1] * b.e[0]);
}

inline std::istream& operator>>(std::istream& is, vec3& v)
{
	is >> v.e[0] >> v.e[1] >> v.e[2];
	return is;
}
inline std::ostream& operator<<(std::ostream& os, const vec3& v)
{
	os << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
	return os;
}

inline vec3 unit_vector(const vec3& v)
{
	return v / v.length();
}

inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

bool refract(const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted)
{
	vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else return false;
}