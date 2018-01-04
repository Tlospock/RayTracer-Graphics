//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "camera.h"

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    /** Camera */
    Triple eye;
    Camera *camera;

    int renderMode; //0 for illumination, 1 for zBuffering, 2 for normal
    float farDistance = 3000;
    float frontDistance = 500;
    bool shadow = false;
	int maxRecursionDepth = 0;
	int superSamplingFactor = 1;

	/** Gooch parameters */
    double b;
    double y;
    double alpha;
    double beta;

public:
    Color trace(const Ray &ray, const int &depth);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setShadow(bool shadow_);
	void setMaxRecursionDepth(int maxRecursDepth);
	void setSuperSamplingFactor(int superSamplingFactor);

    void setRenderMode(int renderModeTemp);
    int getRenderMode();

    void setfarDistance(int farDistanceTemp);
    int getFarDistance();

    void setCamera(Camera *cameraTemp);
    Camera* getCamera();

    void setB(double bTemp);
    double getB();

    void setY(double yTemp);
    double getY();

    void setAlpha(double alphaTemp);
    double getAlpha();

    void setBeta(double betaTemp);
    double getBeta();

    Color illumination(Material *material, Point hit, Vector N, Vector V, Ray ray, const int &depth, Object* objectConsidered);
    Color zBufferImage(Hit min_hit);
    Color normaleBufferImage(Vector N);
    Color textureCoordinate(Point hit, Vector N, Vector V, Ray ray, const int &depth, Object *obj);
    Color goochIllumination(Material *material, Point hit, Vector N, Vector V, Ray ray, const int & depth, Object* objectConsidered);

    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
