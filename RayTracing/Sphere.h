#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"

class Sphere : public Hitable {
public:
	Sphere() {}
	Sphere(Vec3 cen, float r, Material *m) : center(cen), radius(r), mat_ptr(m) {}
	virtual bool Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const;
	Vec3 center;
	float radius;
	Material* mat_ptr;
};

bool Sphere::Hit(const Ray& r, float t_min, float t_max, HitRecord& rec) const {
	Vec3 oc = r.origin() - center;
	float a = dot(r.direction(), r.direction());
	float b = dot(oc, r.direction());
	float c = dot(oc, oc) - radius * radius;
	float discriminant = b * b - a * c;
	rec.mat_ptr = mat_ptr;
	if (discriminant > 0) {
		float temp = (- b - sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
		temp = (-b + sqrt(b * b - a * c)) / a;
		if (temp < t_max && temp > t_min) {
			rec.t = temp;
			rec.p = r.point_at_parameter(rec.t);
			rec.normal = (rec.p - center) / radius;
			return true;
		}
	}
	return false;
}

#endif
