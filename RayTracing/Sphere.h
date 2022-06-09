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
    Vec3 r_dir = r.direction();
    float a = r_dir.length() * r_dir.length();
    float b = dot(oc, r_dir);
    float c = oc.length() * oc.length() - radius * radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.mat_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

#endif
