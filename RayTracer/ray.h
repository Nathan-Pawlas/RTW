#pragma once

#include "Math/Vec3.h"

class ray
{
public:
	ray() {}
	ray(const point3& origin, const vec3& dir) : m_orig(origin), m_dir(dir), tm(0) {}
	ray(const point3& origin, const vec3& dir, double time = 0.0) : m_orig(origin), m_dir(dir), tm(time) {}

	point3 origin() const { return m_orig; }
	vec3 direction() const { return m_dir; }
	double time() const { return tm; }

	point3 at(double t) const 
	{ 
		return m_orig + t * m_dir;
	}


private:
	point3 m_orig;
	vec3 m_dir;
	double tm;
};