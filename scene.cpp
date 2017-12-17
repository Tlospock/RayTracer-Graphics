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
#include <algorithm>

Color Scene::trace(const Ray &ray, const int &depth)
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
        return illumination(material, hit, N, V, ray, depth);
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

/** Phong */
Color Scene::illumination(Material *material, Point hit, Vector N, Vector V, Ray ray, const int &depth)
{
    Color I = Color(0, 0, 0);
	Color reflectionColor = Color(0, 0, 0);
	Vector reflectionColorVector = ray.D - 2*(ray.D.dot(N))*N;

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

        /** Shadow calculation */
        Hit lightHit(std::numeric_limits<double>::infinity(), Vector());

        Object *obj = NULL;

        Ray shadowRay = Ray(hit, L);
        //Point test = shadowRay.at(pow(2.0, -32.0));
        //shadowRay = Ray(test, L);


        for (unsigned int j = 0; j<objects.size(); ++j){
            Hit collisionHit(objects[j]->intersect(shadowRay));

            if (collisionHit.t<lightHit.t){
                lightHit = collisionHit;
                obj = objects[j];
            }
        }

        /** Reflection */
		/*if (depth < maxRecursionDepth) {
			reflectionColor = trace(Ray(hit, reflectionColorVector), depth + 1);
		}*/

        // Point shadowPoint = ray.at(collisionHit.t);
        if(shadow == 0 || !obj || (hit - lights[i]->position).length() <= (hit - ray.at(lightHit.t)).length())
        {
         //cout << "Test shadow!" << shadow << endl;
         /**
         *  Phong ambient Ka * I
         *  https://www.tomdalling.com/blog/modern-opengl/07-more-lighting-ambient-specular-attenuation-gamma/
         */
         Color iAmbient = lightColor * materialColor;

         /**
         *  Phong diffuse Kd * I (L . N), with teta the angle between L & N(diffuse reflection), and phi the angle between R & V(perceived brightness)
         * I ~= cos teta = L . N for normalized L, N
         *
         */
        Color iDiffuse = max(0.0, L.dot(N)) * materialColor;

        /**
         *  Phong specular Ks * I (R . V)^n
         */
        Color iSpecular = pow(max(0.0, V.dot(reflectionVector)), material->n) * lightColor;

        /** Phong illumination */
        I += material->kd * iDiffuse + material->ka * iAmbient + material->ks * iSpecular;
        }
        else
        {
            I += lightColor * materialColor * material->ka;
        }
    }
    /** Reflection */
    if (depth < maxRecursionDepth) {
        reflectionColor = trace(Ray(hit, reflectionColorVector), depth + 1);
    }
	I = I + reflectionColor*material->ks;
    return I;
}

Color Scene::zBufferImage(Hit min_hit)
{
    Color I = Color(0, 0, 0);
    if(min_hit.t < farDistance && min_hit.t > frontDistance)
    {
        float grayValue = 1 - ((min_hit.t - frontDistance) / (farDistance - frontDistance));
        I = Color(grayValue, grayValue, grayValue);

    }
    /* float grayValue = 1 - ((min_hit.t - frontDistance) / (farDistance - frontDistance));
    I = Color(grayValue, grayValue, grayValue); */
    return I;
}

Color Scene::normaleBufferImage(Vector N)
{

    float redValue = (N.x+1)/2;
    float greenValue = (N.y+1)/2;
    float blueValue = (N.z+1)/2;

    Color I = Color(redValue, greenValue, blueValue);

    return I;
}

void Scene::render(Image &img)
{
    int w = img.width();
    int h = img.height();

    /** Following website instructions: http://web.archive.org/web/20110317151403/http://www-graphics.stanford.edu/courses/cs348b-99/viewgeom.html */
    if(camera)
        eye = camera->eye;

    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
			Color col(0, 0, 0);
			//We shoot square(superSamplingFactor) rays inward the pixel.
			for (int i = 0; i<superSamplingFactor; i++){
				//shift from the origin of the pixel. If superSamplingFactor = 0, there's only one ray in the middle of the pixel
				double shiftX = 1 / (2.0*superSamplingFactor) + (double)i / superSamplingFactor;
				for (int j = 0; j<superSamplingFactor; j++) {
					double shiftY = 1 / (2.0*superSamplingFactor) + (double)j / superSamplingFactor;

					if(!camera)
                    {
                        Point pixel(x + shiftX, h - 1 - y + shiftY, 0);
                        Ray ray(eye, (pixel - eye).normalized());
                        col += trace(ray, 0);
                    }
					else
                    {
                        Point pixel(x * camera->up.y + camera->center.x - w / 2, (-y - 1.0) * camera->up.y + camera->center.y + h /2, 0);
                        Ray ray(eye, (pixel - eye).normalized());
                        col += trace(ray, 0);
                    }
				}
			}
			//We calculate the average color obtained by the rays.
			col /= (superSamplingFactor*superSamplingFactor);
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

void Scene::setShadow(bool shadow_){
    shadow = shadow_;
}

void Scene::setMaxRecursionDepth(int maxRecursDepth) {
	maxRecursionDepth = maxRecursDepth;
}


void Scene::setSuperSamplingFactor(int ssFactor) {
	superSamplingFactor = ssFactor;
}

Camera* Scene::getCamera() {
    return camera;
}

void Scene::setCamera(Camera *cameraTemp) {
    camera = cameraTemp;
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
