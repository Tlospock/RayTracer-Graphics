#include "Cylinder.h"
#include <math.h>
#include <vector>
#include <tuple>
#include <algorithm>



Cylinder::Cylinder(Point c1, Point c2, double radius ) : c1(c1), c2(c2), radius(radius)
{
	//A vector representing the cylinder's middle axis
	axis = c2 - c1;
}


Hit Cylinder::intersect(const Ray &ray) {

	/* First we consider the cylinder's sizes, and not the caps
	* The equation of an infinite cylinder of size r oriented among d axis containing the point c is:
	* square(x-c-(d.(x-c)*d)-square(r) = 0
	*
	*The vector's parametric equation is :
	* x(t)=ray.D*t+ray.O
	*
	*So we must solve : 
	* square(ray.D*t+ray.O - c - (d.(ray.D*t+ray.O)*d) - square(r) = 0
	* We can solve this using delta.
	*/


	Vector da = ray.D - ray.D.dot(axis)*axis; //used for less calculations
	Vector dp = ray.O - c1; //used for less calculations
	Vector db = dp - (dp.dot(axis)*axis); //used for less calculations
	double A = da.length_2();
	double B = 2 * (da.dot(db));
	double C = db.length_2()-pow(radius, 2);


	double delta = B * B - 4 * A*C;
	if (delta < 0) return Hit::NO_HIT();
	return Hit(500, ray.D);

	double sol1 = (-B - sqrt(delta)) / (2 * A);
	double sol2 = (-B + sqrt(delta)) / (2 * A);

	/*We choose intersections that are between the caps.*/
	std::vector<tuple<Point, double, bool>> intersections; /*(intersection point, distance from light source, is on the side ?)*/
	std::vector<double> distances;
	Point q1 = ray.O + ray.D*sol1;
	Point q2 = ray.O + ray.D*sol2;
	double dist1 = (ray.O - q1).length();
	double dist2 = (ray.O - q2).length();
	cout << "test : " << sol1 << " " << sol2 << endl;

	//if (sol1 >= 0 && axis.dot(q1 - c1) > 0 && axis.dot(q1 - c2) < 0) {
		//cout << "test1" << endl;
		intersections.push_back(make_tuple(q1, dist1, true));
	//}
	//if (sol2 >= 0 && axis.dot(q2 - c1) > 0 && axis.dot(q2 - c2) < 0) {
		//cout << "test2" << endl;
		intersections.push_back(make_tuple(q2, dist2, true));
	//}

	/*No we look for intersections with the bottom and top planes
	* Our planes equations containing the c1 and c2 points and with d their normal (the cylinder's axis) are :
	* d.(x-ci) = 0
	* 
	*The vector's parametric equation is :
	* x(t)=ray.D*t+ray.O
	*
	* So we must solve : 
	* d.(ray.D*t+ray.O-ci) = 0
	* t = -(d.(ray.O-ci))/(d.ray.D)
	*/
	/*If the ray and the axis are parallel, they ray doesn't hit the caps*/
	/*if(ray.D.dot(axis) != 0){
		double sol3 = -(axis.dot(ray.O - c1)) / (axis.dot(ray.D));
		double sol4 = -(axis.dot(ray.O - c2)) / (axis.dot(ray.D));
		Point q3 = ray.O + ray.D*sol3;
		Point q4 = ray.O + ray.D*sol4;
		double dist3 = (ray.O - q3).length();
		double dist4 = (ray.O - q4).length();
		if (sol3 >= 0 && (q3 - c1).length_2() <= pow(radius, 2)) {
			//cout << "test1" << endl;
			intersections.push_back(make_tuple(q3, dist3, false));
		}
		if (sol4 >= 0 && (q4 - c2).length_2() <= pow(radius, 2)) {
			//cout << "test2" << endl;
			intersections.push_back(make_tuple(q4, dist4, false));
		}
	}*/


	/*We look for the closest point inside those we kept*/
	if (intersections.size() == 0)
		return Hit::NO_HIT();
	cout << "test : " << intersections.size() << endl;
	tuple<Point, double, bool> min = intersections[0];
	double distance = std::get<1>(min);
	for (int i = 1; i < intersections.size(); i++) {
		if (std::get<1>(intersections[i]) < distance) {
			min = intersections[i];
			distance = std::get<1>(min);
		}
	}

	/*When hits the cylinder's side.*/
	if (get<2>(min)) {
		//cout << "test1" << endl;
		return Hit(distance, axis.cross(c1 - std::get<0>(min)).cross(axis));
	}
	/*When hits a cylinder's cap*/
	else {
		//cout << "test2" << endl;
		return ray.D.dot(axis) > 0 ? Hit(distance, -axis) : Hit(distance, axis);
	}

}

Point Cylinder::localPoint(Point globalPoint) {
	return globalPoint;
}