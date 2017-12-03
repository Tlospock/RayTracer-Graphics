#include "Triangle.h"
#include <iostream>
#include <math.h>

/************************** Triangle **********************************/

Hit Triangle::intersect(const Ray &ray)
{
    double t;
	Point intersection;


    /**Just like a plan, we have one normal, and if it crosses the plan formed by the triangle, we fine the intersection point**/
	if (ray.D.dot(normal) == 0) {
        return Hit::NO_HIT();
	}

	double t2 = -(normal.dot(ray.O)+d)/normal.dot(ray.D);
	intersection = ray.O + (ray.D*t2);

    /**We check if the intersection is inside the triangle**/
    Vector perpendicular = edge1.cross(intersection-point1);
    if(perpendicular.dot(normal)<0) return Hit::NO_HIT();

    perpendicular = edge2.cross(intersection-point2);
    if(perpendicular.dot(normal)<0) return Hit::NO_HIT();

    perpendicular = edge3.cross(intersection-point3);
    if(perpendicular.dot(normal)<0) return Hit::NO_HIT();

    t = (ray.O-intersection).length();
	return Hit(t, normal);
}
