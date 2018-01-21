#pragma once
#include "object.h"
#include "triple.h"

class Cylinder : public Object
{
	public:
		Cylinder(Point p1, Point p2, double r);

		virtual Hit intersect(const Ray &ray);
		virtual Point localPoint(Point globalPoint);

		const Point c1; //middles of both circle bases.
		const Point c2;
		const double radius;
		Vector axisVector, zeroVector, sideVector;

	private:
		Vector axis;
};

