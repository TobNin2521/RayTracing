#ifndef DIELECTRICH
#define DIELECTRICH

#include "Material.h"

class Dielectric :public Material {
public:
	Dielectric(float ri) : ref_idx(ri) {}
	virtual bool Scatter(const Ray& r_in,const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outward_normal;
		Vec3 reflected = Reflect(r_in.direction(), rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 0.0);
		Vec3 refracted;
		if (dot(r_in.direction(), rec.normal) > 0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
		}
		if (Refract(r_in.direction(), outward_normal, ni_over_nt, refracted)) {
			scattered = Ray(rec.p, refracted);
		}
		else {
			scattered = Ray(rec.p, refracted);
			return false;
		}
		return true;
	}
	float ref_idx;
};

#endif
