#pragma once

#include "../utils.h"
#include "../hittable.h"
#include "../hittable_list.h"
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

	static inline shared_ptr<hittable_list> box(const point3& a, const point3& b, shared_ptr<material> mat)
	{
		//A list of quads that will make up the (6) sides of a cube
		auto sides = make_shared<hittable_list>();

		auto min = point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
		auto max = point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

		
		auto dx = vec3(max.x() - min.x(), 0, 0);
		auto dy = vec3(0, max.y() - min.y(), 0);
		auto dz = vec3(0, 0, max.z() - min.z());

		sides->add(make_shared<quad>(point3(min.x(), min.y(), max.z()), dx, dy, mat)); // front
		sides->add(make_shared<quad>(point3(max.x(), min.y(), max.z()), -dz, dy, mat)); // right
		sides->add(make_shared<quad>(point3(max.x(), min.y(), min.z()), -dx, dy, mat)); // back
		sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dz, dy, mat)); // left
		sides->add(make_shared<quad>(point3(min.x(), max.y(), max.z()), dx, -dz, mat)); // top
		sides->add(make_shared<quad>(point3(min.x(), min.y(), min.z()), dx, dz, mat)); // bottom
		
		return sides;
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