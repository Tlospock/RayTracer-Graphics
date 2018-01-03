#pragma once
#include "object.h"
#include "triple.h"
class Plane :
	public Object
{
public:
	Plane(Point p1, Point p2, Point p3);

	virtual Hit intersect(const Ray &ray);
    virtual Point localPoint(Point globalPoint);

	const Point p1;
	const Point p2;
	const Point p3;
	Vector axisVector, zeroVector, sideVector;

private:
	Vector normal;
	double d;
};

