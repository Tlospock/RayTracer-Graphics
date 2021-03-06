//
//  Framework for a raytracer
//  File: sphere.cpp
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

#include "sphere.h"
#include <iostream>
#include <math.h>
#include <vector>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return Hit::NO_HIT().
    * Otherwise, return an instance of Hit() with the distance of the
    * intersection point from the ray origin as t and the normal ad N (see example).
    ****************************************************/

    // place holder for actual intersection calculation




    /*Our sphere's equation is square(||x-position||)-square(r)=0
    *The vector's parametric equation is :
    * X(t)=ray.D*t+ray.O
    *
    * So we must solve :
    * square(||ray.D*t+ray.O-position||) - square(r)  = 0
    * <=> square(||ray.D||)*square(t)+2*ray.D*(ray.O-position)*t+square(||ray.O-position||)-square(r) = 0
    * We can solve this easily using delta.
    * Once it's done, we use the solution in the vector's parametric equation to find the intersection(s)
    * Then we can calculate the distance between the closest intersection and the ray origins
    */

    double t;
    Point intersection;

    double A = ray.D.length_2();
    double B = 2*(ray.D.dot(ray.O - position));
    double C = (ray.O-position).length_2()-r*r;

    double delta = pow(B,2)-(4*A*C);
    if(delta < 0){
        //no intersection
        return Hit::NO_HIT();
    }
    else if(delta == 0){
        //1 intersection
        double solution = -B/(2*A);
        intersection = (ray.D * solution) + ray.O;
        double distance = (ray.O-intersection).length();
        t = solution;
    }
    else{
        //2 intersections
        double solution1 = (-B-sqrt(delta))/(2*A);
        double solution2 = (-B+sqrt(delta))/(2*A);
        Point inters1 = (ray.D * solution1) + ray.O;
        Point inters2 = (ray.D * solution2) + ray.O;
        double distance1 = (ray.O-inters1).length();
        double distance2 = (ray.O-inters2).length();
        t = (solution1 < solution2 && solution2 >= 0) ? solution1 : solution2;

        /** if(distance1 >=0)
        {
            t = (distance1 < distance2) ? distance1 : distance2;
        } */

        // intersection = (distance1 < distance2) ? inters1 : inters2;
        intersection = (t == solution1) ? inters1 : inters2;
    }


    /*Vector OC = (position - ray.O).normalized();
    if (OC.dot(ray.D) < 0.999) {
        return Hit::NO_HIT();
    }

    double t = 1000;*/

    if(t <= 0)
    {
        return Hit::NO_HIT();
    }

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the inters
    ****************************************************/


    Vector N = Vector(intersection - position);
    return Hit(t,N);
}

Point Sphere::localPoint(Point globalPoint)
{
    Point localCartesianSpherePoint = globalPoint - position;

	if (localCartesianSpherePoint.length() == 0)
		return Color(0, 0, 0);
	if (rotationAngle != 0) {
		//Here is the vector that must become (0,0,1) with the new cartesian coordinates system
		Vector verticalVector = (r * rotationVector).normalized();
		Vector Zaxis(0, 0, 1);

		Vector R = verticalVector.cross(Zaxis).normalized(); //Rotation axis
		double alpha = acos(verticalVector.dot(Zaxis)); //rotation angle

		//Now we calculate the new cartesian coordinates of our sphere points, with the rotation
		localCartesianSpherePoint = cos(alpha)*localCartesianSpherePoint + (1 - cos(alpha))*localCartesianSpherePoint.dot(R)*R + sin(alpha)*R.cross(localCartesianSpherePoint);
	}
	

    double m_pi = 3.14159265358979323846264338327950288419716939937510;
    double rho = r;
    double phi = (acos(localCartesianSpherePoint.z/rho)) / m_pi;
    double theta = (atan2(localCartesianSpherePoint.y, localCartesianSpherePoint.x)-rotationAngle*m_pi/180) / (2*m_pi);
	while (theta < 0) theta += 1;

    Point localSphericalPoint(rho, theta, phi);
    return localSphericalPoint;
}