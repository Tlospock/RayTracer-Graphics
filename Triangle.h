#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "object.h"

class Triangle : public Object
{
    public:
        Triangle(Point point1, Point point2, Point point3) : point1(point1), point2(point2), point3(point3){
            edge1 = point2-point1;
            edge2 = point3-point2;
            edge3 = point1-point3;
            /* We need to get a normal vector to our triangle
            We transform our points into two vectors (P1P2) and (P2P3) and cross them to get a normal vector
            */
            Vector v= point2-point1;
            normal = v.cross(point3-point1);
            /*d is a value needed for our plane's equation :
            ax+by+cz+d=0*/
            d =-point1.x*normal.x-point1.y*normal.y-point1.z*normal.z;
        }

        virtual Hit intersect(const Ray &ray);


        const Point point1, point2, point3;


    private:
        Vector normal;
        double d;
        Point edge1, edge2, edge3;
};

#endif // TRIANGLE_H
