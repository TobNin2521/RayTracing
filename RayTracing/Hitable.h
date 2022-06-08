#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"
#include "Walnut/Random.h"
#include "Walnut/Timer.h"

class Material;

struct HitRecord {
	float t;
	Vec3 p;
	Vec3 normal;
	Material *mat_ptr;
};

Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool Refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
	Vec3 uv = unit_vector(v);
	float dt = dot(uv, n);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	else {
		return false;
	}
}

class Hitable {
public:
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const = 0;
};

#endif