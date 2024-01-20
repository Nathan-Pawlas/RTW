#pragma once

#include "../hittable.h"
#include "../Math/vec3.h"

class sphere : public hittable
{
public:
    sphere(point3 _center, double _radius, shared_ptr<material> _mat) : center(_center), radius(_radius), mat(_mat) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;

        // Sphere Intersection: Quadratic Formula, (-h +- sqrt(h^2 - ac))/a
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius * radius;

        auto discriminant = half_b * half_b - a * c;
        if (discriminant < 0) return false; //No intersection if discriminant is < 0
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        //Determine if Normal is in/outside sphere
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};