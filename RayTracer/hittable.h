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

class translate : public hittable
{
public:
	translate(shared_ptr<hittable> p, const vec3& displacement) : object(p), offset(displacement)
	{
		bbox = object->bounding_box() + offset;
	}
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		ray offset_r(r.origin() - offset, r.direction(), r.time());

		if (!object->hit(offset_r, ray_t, rec))
			return false;

		rec.p += offset;

		return true;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> object;
	vec3 offset;
	aabb bbox;
};

class rotate_y : public hittable
{
public:
	rotate_y(shared_ptr<hittable> p, double angle) : object(p)
	{
		auto radians = degrees_to_radians(angle);
		sin_theta = sin(radians);
		cos_theta = cos(radians);
		bbox = object->bounding_box();

		point3 min(infinity, infinity, infinity);
		point3 max(-infinity, -infinity, -infinity);

		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				for (int k = 0; k < 2; k++)
				{
					auto x = i * bbox.x.max + (1 - i) * bbox.x.min;
					auto y = j * bbox.y.max + (1 - j) * bbox.y.min;
					auto z = k * bbox.z.max + (1 - k) * bbox.z.min;

					auto newx = cos_theta * x + sin_theta * z;
					auto newz = -sin_theta * x + cos_theta * z;

					vec3 tester(newx, y, newz);

					
				}
			}
		}
	}
	bool hit(const ray& r, interval ray_t, hit_record& rec) const override
	{
		//Ray World space -> Object space
		auto origin = r.origin();
		auto dir = r.direction();

		origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
		origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

		dir[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
		dir[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

		ray rotated_ray(origin, dir, r.time());

		//Check for intersection
		if (!object->hit(rotated_ray, ray_t, rec))
			return false;
		
		//Int Point Object space -> World space
		auto p = rec.p;
		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

		//Normal Object space -> World space
		auto norm = rec.normal;
		norm[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
		norm[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

		rec.p = p;
		rec.normal = norm;

		return true;
	}

	aabb bounding_box() const override { return bbox; }

private:
	shared_ptr<hittable> object;
	double sin_theta;
	double cos_theta;
	aabb bbox;
};