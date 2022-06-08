#ifndef METALH
#define METALH

#include "Material.h"

class Metal : public Material {
public:
	Metal(const Vec3& a, float f = 0.8) : albedo(a) { if (f < 1) fuzz = f; else fuzz = 1; }
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 reflected = Reflect(unit_vector(r_in.direction()), rec.normal);
		scattered = Ray(rec.p, reflected + fuzz * random_in_unit_sphere2());
		attenuation = albedo;
		return (dot(scattered.direction(), rec.normal) > 0);
	}
	Vec3 albedo;
	float fuzz;
};

#endif