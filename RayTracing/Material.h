#ifndef MATERIALH
#define MATERIALH

#include "Hitable.h"

Vec3 random_in_unit_sphere2() {
	Vec3 p;
	int i = 0;
	do {
		p = 2 * Vec3(drand48(), drand48(), drand48()) - Vec3(1, 1, 1);
	} while (p.squared_length() >= 1);
	return p;
}

class Material {
public:
	virtual bool Scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
};

#endif
