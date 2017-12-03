#pragma once
#include "object.h"
#include "triple.h"
class Plane :
	public Object
{
public:
	Plane(Point p1, Point p2, Point p3);

	virtual Hit intersect(const Ray &ray);


	const Point p1;
	const Point p2;
	const Point p3;

private:
	Vector normal;
	double d;
};

