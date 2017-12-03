#include "Plane.h"
#include <iostream>
#include <math.h>

Hit Plane::intersect(const Ray &ray)
{

	double t;
	Point intersection;

	/* We need to get a normal vector to our plane
	We transform our points into two vectors (P1P2) and (P2P3) and cross them to get a normal vector
	*/
	Vector v1 = p2 - p1;
	Vector v2 = p3 - p1;
	Vector n = v1.cross(v2);

	if (ray.D.dot(n) == 0) {
		/*When the ray and the plan are parallel*/
		if (n.dot(p1 - ray.O) == 0)
			/*When the ray belongs to the plan*/
			return Hit(0, n);
		else
			/*When the ray doesn't hit the plan*/
			return Hit::NO_HIT();
	}
	double d= -p1.x*n.x-p1.y*n.y-p1.z*n.z;
	double sol = -(n.dot(ray.O)+d)/n.dot(ray.D);
	intersection = ray.O + (ray.D*sol);

	t = (ray.O-intersection).length();
	return Hit(t, n);
}
