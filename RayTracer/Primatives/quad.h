#pragma once

#include "../utils.h"
#include "../hittable.h"
#include <cmath>

class quad : public hittable
{
public:
	quad(const point3& _q, const vec3& _u, const vec3& _v, shared_ptr<material> _mat)
		: q(_q), u(_u), v(_v), mat(_mat)
	{
		auto n = cross(u, v);
		normal = unit_vector(n);
		d = dot(normal, q);
		w = n / dot(n, n);

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
		auto denomenator = dot(normal, r.direction());

		//If parallel, return false
		if (fabs(denomenator) < 1e-8)
			return false;

		//Make sure the point is within t is inside the ray interval
		auto t = (d - dot(normal, r.origin())) / denomenator;
		if (!ray_t.contains(t))
			return false;

		//Make sure the hit point is within the plane coordinates (alpha, beta)
		auto intersection = r.at(t);
		vec3 planar_hitpt_vec = intersection - q;
		auto alpha = dot(w, cross(planar_hitpt_vec, v));
		auto beta = dot(w, cross(u, planar_hitpt_vec));
		if (!is_interior(alpha, beta, rec))
			return false;


		rec.t = t;
		rec.p = r.at(t);
		rec.mat = mat;
		rec.set_face_normal(r, normal);

		return true;
	}

private:
	point3 q; //Origin (bottom left corner)
	vec3 u, v;
	shared_ptr<material> mat;
	aabb bbox;
	vec3 normal;
	double d;
	vec3 w;

	virtual bool is_interior(double a, double b, hit_record& rec) const
	{
		//Given a point in plane coords, return false if outside the quad, otherwise
		//set UV coords and return true

		if ((a < 0) || (a > 1) || (b < 0) || (b > 1))
			return false;

		rec.u = a;
		rec.v = b;
		return true;
	}
};