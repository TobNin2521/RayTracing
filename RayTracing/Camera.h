#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

Vec3 random_unit_in_disk() {
	Vec3 p;
	do {
		p = 2 * Vec3(drand48(), drand48(), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1);
	return p;
}

class Camera {
public:
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float fov, float aspect, float aperture, float focus_dist) {
		lens_radius = aperture / 2;
		float theta = fov * M_PI / 180;
		float half_heigth = tan(theta / 2);
		float half_width = aspect * half_heigth;
		origin = lookfrom;

		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width * focus_dist * u - half_heigth * focus_dist * v - w * focus_dist;
		horizontal = 2 * half_width * focus_dist * u;
		vertical = 2 * half_heigth * focus_dist * v;
	}
	Ray get_ray(float s, float t) { 
		Vec3 rd = lens_radius * random_unit_in_disk();
		Vec3 offset = u * rd.x() - v * rd.y();
		return Ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset); 
	}
	Vec3 origin;
	Vec3 lower_left_corner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lens_radius;
};

#endif
