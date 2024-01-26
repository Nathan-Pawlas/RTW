#pragma once

#include "utils.h"
#include "hittable.h"
#include "texture.h"

class hit_record;

class material
{
public:
	virtual ~material() = default;

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
public:
	lambertian(const color& a) : albedo(make_shared<solid_color>(a)) {}
	lambertian(shared_ptr<texture> t) : albedo(t) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		auto scatter_dir = rec.normal + random_unit_vector();
		if (scatter_dir.near_zero())
			scatter_dir = rec.normal;
		scattered = ray(rec.p, scatter_dir, r_in.time());
		attenuation = albedo->value(rec.u, rec.v, rec.p);
		return true;
	}

private:
	shared_ptr<texture> albedo;
};

class metal : public material
{
public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = ray(rec.p, reflected + fuzz * random_unit_vector(), r_in.time());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
private:
	color albedo;
	double fuzz;
};

class dielectric : public material
{
public:
	dielectric(double ir) : index_refraction(ir) {}

	bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
	{
		attenuation = color(1.0, 1.0, 1.0);
		double refraction_ratio = rec.front_face ? (1.0 / index_refraction) : index_refraction;

		vec3 unit_dir = unit_vector(r_in.direction());
		double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		bool cannot_refract = refraction_ratio * sin_theta > 1.0;
		vec3 dir;

		if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
			dir = reflect(unit_dir, rec.normal);
		else
			dir = refract(unit_dir, rec.normal, refraction_ratio);
		scattered = ray(rec.p, dir, r_in.time());
		return true;
	}
private:
	double index_refraction;

	static double reflectance(double cosine, double ref_indx)
	{
		auto r0 = (1 - ref_indx) / (1 + ref_indx);
		r0 *= r0;
		return r0 + (1 - r0) * pow((1 - cosine), 5);
	}
};