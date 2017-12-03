#pragma once
#include "object.h"
class Plane :
	public Object
{
public:
	Plane(Point p1, Point p2, Point p3) : p1(p1), p2(p2), p3(p3) {}
	
	//virtual Hit intersect(const Ray &ray);


	const Point p1;
	const Point p2;
	const Point p3;
};

