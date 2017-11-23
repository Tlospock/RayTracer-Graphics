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




    /*Our sphere's equation is square(position.x)+square(position.y)+square(position.z)-square(r)=0
    *The vector's parametric equation is :
    * x(t)=ray.D.x*t+ray.O.x
    * y(t)=ray.D.y*t+ray.O.y
    * z(t)=ray.D.z*t+ray.O.z
    *
    * So we must solve :
    * (ray.D.x*t+ray.O.x)²+(ray.D.y*t+ray.O.y)²+(ray.D.z*t+ray.O.z)-(r*r)=0
    * <=> (D.x²+D.y²+D.z²)*t² + 2*(D.x*O.x+D.y*O.y+D.z*O.z)*t + O.x²+O.y²+O.z²-r² = 0
    * delta = (2*(D.x*O.x+D.y*O.y+D.z*O.z))² - 4*(D.x²+D.y²+D.z²)*(O.x²+O.y²+O.z²-r²)
    */
    //Vector OC = (position - ray.O).normalized();

    double A = ray.D.length_2();
    double B = 2*(ray.D.dot(OC));
    double C = OC.length_2()-r*r-;

    //double A = pow(ray.D.x,2)+pow(ray.D.y,2)+pow(ray.D.z,2);
    //double B = 2*(ray.D.x*OC.x+ray.D.y*OC.y+ray.D.z*OC.z);
    //double C = pow(OC.x,2)+pow(OC.y,2)+pow(OC.z,2)-r*r;

    Vector N;

    double delta = pow(B,2)-(4*A*C);
    if(delta > 0){
        return Hit::NO_HIT();
    }
    else if(delta == 0){
        double solution = -B/(2*A);
        Point inters = (ray.D * solution) + OC;
        double distance = sqrt(pow((ray.D.x-inters.x), 2) + pow((ray.D.y-inters.y), 2) + pow((ray.D.z-inters.z),2));
        return Hit(distance, N);
    }
    else{
        double solution1 = (-B-sqrt(delta))/(2*A);
        double solution2 = (-B+sqrt(delta))/(2*A);
        Point inters1 = (ray.D * solution1) + OC;
        Point inters2 = (ray.D * solution2) + OC;
        double distance1 = sqrt(pow((ray.D.x-inters1.x), 2) + pow((ray.D.y-inters1.y), 2) + pow((ray.D.z-inters1.z),2));
        double distance2 = sqrt(pow((ray.D.x-inters2.x), 2) + pow((ray.D.y-inters2.y), 2) + pow((ray.D.z-inters2.z),2));
        return (distance1 < distance2) ? Hit(distance1, N) : Hit(distance2, N);
    }


    /*Vector OC = (position - ray.O).normalized();
    if (OC.dot(ray.D) < 0.999) {
        return Hit::NO_HIT();
    }
    double t = 1000;*/
    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    *
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/


    //Vector N /* = ... */;

    return Hit(t,N);
}
