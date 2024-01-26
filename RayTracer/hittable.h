#pragma once

#include "utils.h"
#include "aabb.h"

class material;

class hit_record
{
public:
	void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		//If dot product is negative then normal is outside sphere, inside otherwise
		front_face = dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

public:
	point3 p;
	vec3 normal;
	shared_ptr<material> mat;
	double t;
	double u;
	double v;
	bool front_face;
};


class hittable
{
public:
	virtual ~hittable() = default;

	virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

	virtual aabb bounding_box() const = 0;
};