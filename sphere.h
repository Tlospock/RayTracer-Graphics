//
//  Framework for a raytracer
//  File: sphere.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#ifndef SPHERE_H_115209AE
#define SPHERE_H_115209AE

#include "object.h"

class Sphere : public Object
{
public:

	Sphere(Point position, double r, Vector rotationVector, double angle) : position(position), r(r), rotationVector(rotationVector), rotationAngle(angle) {
		axisVector = Vector(0, 0, r);
		zeroVector = Vector(0, r, 0);
		sideVector = Vector(r, 0, 0);
	}

    virtual Hit intersect(const Ray &ray);
    virtual Point localPoint(Point globalPoint);

    const Point position;
    const double r;
	const Vector rotationVector;
	const double rotationAngle;
	Vector axisVector, zeroVector, sideVector;
};

#endif /* end of include guard: SPHERE_H_115209AE */
