//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

    /** Normalizing N*/
    N.normalize();

    /****************************************************
    * This is where you should insert the color
    * calculation (Phong model).
    *
    * Given: material, hit, N, V, lights[]
    * Sought: color
    *
    * Hints: (see triple.h)
    *        Triple.dot(Vector) dot product
    *        Vector+Vector      vector sum
    *        Vector-Vector      vector difference
    *        Point-Point        yields vector
    *        Vector.normalize() normalizes vector, returns length
    *        double*Color        scales each color component (r,g,b)
    *        Color*Color        dito
    *        pow(a,b)           a to the power of b
    ****************************************************/

    if(renderMode == 0) /** If we do illumination */
    {
        return illumination(material, hit, N, V);
    }
    else if(renderMode == 1) /** z-buffer image */
    {
        return zBufferImage(min_hit);
    }
    else if(renderMode == 2) /** Normal buffer image */
    {
        return normaleBufferImage(N);
    }
    else
    {
        return material->color;
    }
}

Color Scene::illumination(Material *material, Point hit, Vector N, Vector V)
{
    Color I = Color(0, 0, 0);

    for(unsigned int i=0; i<lights.size(); ++i)
    {
        /** Vector to light */
        Vector L = lights[i]->position - hit;

        /** Vector of reflected light */
        Vector reflectionVector = 2*(L.dot(N))*N -L;
        reflectionVector.normalize();

        // Vector of refraction (to do later)

        /** Normalization of light vector*/
        L.normalize();

        /** Get light color */
        Color lightColor = lights[i]->color;
        Color materialColor = material->color;

        /**
         *  Phong diffuse Kd * I (L . N), with teta the angle between L & N(diffuse reflection), and phi the angle between R & V(perceived brightness)
         * I ~= cos teta = L . N for normalized L, N
         *
         */
        Color iDiffuse = max(0.0, L.dot(N)) * materialColor;

        /**
         *  Phong ambient Ka * I
         *  https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
         */
         Color iAmbient = lightColor * materialColor;

        /**
         *  Phong specular Ks * I (R . V)^n
         */
        Color iSpecular = pow(max(0.0, V.dot(reflectionVector)), material->n) * lightColor;

        /** Phong illumination */
        I += material->kd * iDiffuse + material->ka * iAmbient + material->ks * iSpecular;

    }

    return I;
}

Color Scene::zBufferImage(Hit min_hit)
{
    float grayValue = 1 - ((min_hit.t - frontDistance) / (farDistance - frontDistance));
    Color I = Color(grayValue, grayValue, grayValue);

    return I;
}

Color Scene::normaleBufferImage(Vector N)
{

    float redValue = N.x;
    float greenValue = N.y;
    float blueValue = N.z;

    Color I = Color(redValue, greenValue, blueValue);

    return I;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);
            col.clamp();
            img(x,y) = col;
        }
    }
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

int Scene::getRenderMode()
{
    return renderMode;
}

void Scene::setRenderMode(int renderModeTemp)
{
    renderMode = renderModeTemp;
}

void Scene::setfarDistance(int farDistanceTemp)
{
    farDistance = farDistanceTemp;
}

int Scene::getFarDistance()
{
    return farDistance;
}
