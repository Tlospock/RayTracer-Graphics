#include "Plane.h"
#include <iostream>
#include <math.h>

Plane::Plane(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3), normal(0,0,0), d(0){
        /* We need to get a normal vector to our plane
        We transform our points into two vectors (P1P2) and (P2P3) and cross them to get a normal vector
        */
        Vector v= p2-p1;
		normal = v.cross(p3-p1);
		/*d is a value needed for our plane's equation :
		ax+by+cz+d=0*/
        d =-p1.x*normal.x-p1.y*normal.y-p1.z*normal.z;
}

Hit Plane::intersect(const Ray &ray)
{

	double t;
	Point intersection;


	if (ray.D.dot(normal) == 0) {
		/*When the ray and the plan are parallel*/
		if (normal.dot(p1 - ray.O) == 0)
			/*When the ray belongs to the plan*/
			return Hit(0, normal);
		else
			/*When the ray doesn't hit the plan*/
			return Hit::NO_HIT();
	}
	/*To find the intersection point, we solve this system :
	x = Ray.O.x+Ray.D.x*t
	y = Ray.O.y+Ray.D.y*t
	z = Ray.O.z+Ray.D.z*t
	a*x+b*y+c*z+d = 0
	with (a,b,c) the normal vector
	*/
	double t2 = -(normal.dot(ray.O)+d)/normal.dot(ray.D);
	intersection = ray.O + (ray.D*t2);

	t = (ray.O-intersection).length();
	return Hit(t, normal);
}
