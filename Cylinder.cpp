#include "Cylinder.h"
#include <math.h>
#include <vector>
#include <tuple>
#include <algorithm>



Cylinder::Cylinder(Point c1, Point c2, double radius ) : c1(c1), c2(c2), radius(radius)
{
	//A vector representing the cylinder's middle axis
	axis = (c2 - c1).normalized();
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
	std::vector<tuple<Point, double, bool>> intersections; /*(intersection point, distance from light source, is on the cylinder's side ?)*/
	double e = 0.00001;
	Vector da = ray.D - ray.D.dot(axis)*axis; //used for less calculations
	Vector dp = ray.O - c1; //used for less calculations
	Vector db = dp - (dp.dot(axis)*axis); //used for less calculations
	double A = da.length_2();
	double B = 2 * (da.dot(db));
	double C = db.length_2()-pow(radius, 2);

	double delta = B * B - 4 * A*C;

	if (delta < -e) return Hit::NO_HIT();
	else if (delta < e) {
		double sol = -B / (2 * A);
		Point q = ray.O + ray.D*sol;
		if (sol >= 0 && axis.dot(q - c1) > 0 && axis.dot(q - c2) < 0) {
			intersections.push_back(make_tuple(q, sol, true));
		}
	}
	else {
		double sol1 = (-B - sqrt(delta)) / (2 * A);
		double sol2 = (-B + sqrt(delta)) / (2 * A);

		/*We choose intersections that are between the caps.*/
		Point q1 = ray.O + ray.D*sol1;
		Point q2 = ray.O + ray.D*sol2;

		if (sol1 >= e && axis.dot(q1 - c1) > 0 && axis.dot(q1 - c2) < 0) {
			intersections.push_back(make_tuple(q1, sol1, true));
		}
		if (sol2 >= e && axis.dot(q2 - c1) > 0 && axis.dot(q2 - c2) < 0) {
			intersections.push_back(make_tuple(q2, sol2, true));
		}
	}

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
	if(ray.D.dot(axis) != 0){
		double sol3 = -(axis.dot(ray.O - c1)) / (axis.dot(ray.D));
		double sol4 = -(axis.dot(ray.O - c2)) / (axis.dot(ray.D));
		Point q3 = ray.O + ray.D*sol3;
		Point q4 = ray.O + ray.D*sol4;
		if (sol3 >= e && (q3 - c1).length_2() - pow(radius, 2) <= e) {
			//cout << "test1" << endl;
			intersections.push_back(make_tuple(q3, sol3, false));
		}
		if (sol4 >= e && (q4 - c2).length_2() - pow(radius, 2) <= e) {
			//cout << "test2" << endl;
			intersections.push_back(make_tuple(q4, sol4, false));
		}
	}


	/*We look for the closest point inside those we kept*/
	if (intersections.size() == 0)
		return Hit::NO_HIT();
	//cout << "test : " << intersections.size() << endl;
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
		return ray.D.dot(axis) > e ? Hit(distance, -axis) : Hit(distance, axis);
	}

}

Point Cylinder::localPoint(Point globalPoint) {
	return globalPoint;
}