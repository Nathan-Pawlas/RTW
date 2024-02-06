#pragma once

#include "../utils.h"
#include "../hittable.h"

class quad : public hittable
{
public:
	quad(const point3& _q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat)
		: q(_q), u(_u), v(_v), mat(_mat)
	{
		set_bounding_box();
	}

	virtual void set_bounding_box()
	{
		bbox = aabb(q, q + u + v).pad();
	}

	aabb bounding_box() const override
	{
		return bbox;
	}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		return false;
	}

private:
	point3 q; //Origin (bottom left corner)
	vec3 u, v;
	shared_ptr<material> mat;
	aabb bbox;
};