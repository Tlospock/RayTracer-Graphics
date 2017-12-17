//
//  Framework for a raytracer
//  File: raytracer.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html
//

#include "raytracer.h"
#include "object.h"
#include "sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);
std::vector<int> parseDouble(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
    return t;
}

std::vector<int> parseDouble(const YAML::Node& node)
{
    std::vector<int> t;
    t.push_back(node[0]);
    t.push_back(node[1]);
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
    node["color"] >> m->color;
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);
        returnObject = sphere;
    }
	if (objectType == "plane") {
		Point p1, p2, p3;
		node["point1"] >> p1;
		node["point2"] >> p2;
		node["point3"] >> p3;
		Plane *plane = new Plane(p1, p2, p3);
		returnObject = plane;
	}
	if (objectType == "triangle") {
		Point p1, p2, p3;
		node["point1"] >> p1;
		node["point2"] >> p2;
		node["point3"] >> p3;
		Triangle *triangle = new Triangle(p1, p2, p3);
		returnObject = triangle;
	}

    if (returnObject) {
        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read scene configuration options
            if(doc.FindValue("Eye"))
                scene->setEye(parseTriple(doc["Eye"]));
            else
            {
                Point eye, center;
                Vector up;
                doc["Camera"]["eye"] >> eye;
                doc["Camera"]["center"] >> center;
                doc["Camera"]["up"] >> up;
                const YAML::Node& nodeTemp = doc["Camera"]["viewSize"];
                int w, h;
                nodeTemp[0] >> w;
                nodeTemp[1] >> h;
                Camera* c = new Camera(eye, center, up, w, h);
                scene->setCamera(c);
            }


			//cout << scene->getCamera()->width << " " << scene->getCamera()->height << endl;

			if (doc.FindValue("shadow"))
				scene->setShadow(doc["shadow"]);
			if (doc.FindValue("MaxRecursionDepth"))
				scene->setMaxRecursionDepth(doc["MaxRecursionDepth"]);
			scene->setSuperSamplingFactor(doc["SuperSampling"]["factor"]);

            /** Read scene mode (0 for illumination, 1 for Zbuffer image, 2 for normal bufferImage */
            if(doc.FindValue("renderMode"))
                scene->setRenderMode(doc["renderMode"]);

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->getCamera()->width,scene->getCamera()->height);
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
