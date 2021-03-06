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
#include "plane.h"
#include "triangle.h"
#include "cylinder.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include "camera.h"
#include "mesh.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>
#include <cstdlib>
#include <ctime>

Raytracer::Raytracer()
{
	srand(time(0));
}

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

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

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    std::string texture;
    Material *m = new Material();
    if (node.FindValue("color")) {
        node["color"] >> m->color;
        m->texture = NULL;
    } else if (node.FindValue("texture")) {
        node["texture"] >> texture;
        m->texture = new Image(texture.c_str());
    } else {
        cout << "An object is missing a color or texture\n";
        exit(1);
    }
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
    } else if (objectType == "plane") {
        Point pos;
        Vector normal;
        node["position"] >> pos;
        node["normal"] >> normal;
        Plane *plane = new Plane(pos, normal);
        returnObject = plane;
    } else if (objectType == "triangle") {
        Point pointA, pointB, pointC;
        node["pointA"] >> pointA;
        node["pointB"] >> pointB;
        node["pointC"] >> pointC;
        Triangle *triangle = new Triangle(pointA, pointB, pointC);
        returnObject = triangle;
    } else if (objectType == "cylinder") {
		Point pos;
		double r, h;
		node["position"] >> pos;
		node["radius"] >> r;
		node["height"] >> h;
		Cylinder *c = new Cylinder(pos, r, h);
		returnObject = c;
	} else if (objectType == "mesh") {
        std::string filename;
        float scale;
		Point pos;
        node["file"] >> filename;
        node["scale"] >> scale;
		node["position"] >> pos;
        Mesh *m = new Mesh(filename, scale, pos);
        returnObject = m;
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

Camera* Raytracer::parseCamera(const YAML::Node& node)
{
	Point p, c;
	Vector u;
	int width, height;
	node["eye"] >> p;
	node["center"] >> c;
	node["up"] >> u;
	node["viewSize"][0] >> width;
	node["viewSize"][1] >> height;
	return new Camera(p, c, u, width, height);
}

Camera* Raytracer::parseEyeAsCamera(const YAML::Node & node)
{
	Point p;
	node >> p;
	return new Camera(p, Triple(p.x,p.y,0), Triple(0,1,0), 400, 400);
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

            // Read the render mode, default to Phong illumination
            try {
                std::string m;
                doc["RenderMode"] >> m;
                if (m == "normal")
                    scene->setMode(NORMAL);
                else if (m == "zbuffer")
                    scene->setMode(ZBUFFER);
                else if (m == "phong")
                    scene->setMode(PHONG);
                 else if (m == "gooch")
                    scene->setMode(GOOCH);
                    parseGoochParameters(doc["GoochParameters"]);
            } catch (YAML::TypedKeyNotFound<std::string> &e) {
                scene->setMode(PHONG);
            }

			// Read shadow usage
			try {
				bool shadows;
				doc["Shadows"] >> shadows;
				scene->setShadows(shadows);
			} catch (YAML::TypedKeyNotFound<std::string> &e) {
				scene->setShadows(false);
			}

            // Read recursion depth
            try {
                unsigned int depth;
                doc["MaxRecursionDepth"] >> depth;
                scene->setRecursionDepth(depth);
            } catch (YAML::TypedKeyNotFound<std::string> &e) {
                cout << "Using default ray recursion depth\n";
                scene->setRecursionDepth(0);
            }

            // Read supersampling settings
            try {
                const YAML::Node& superSampling = doc["SuperSampling"];
                unsigned int factor;
				std::string m;
				SUPERSAMPLING_MODE mode;
                superSampling["factor"] >> factor;
				// Read mode
				try {
					superSampling["mode"] >> m;
					if (m == "grid") {
						mode = GRID;
						cout << "Using grid supersampling mode\n";
					} else if (m == "jitter") {
						mode = JITTER;
						cout << "Using jitter supersampling mode\n";
					} else if (m == "random") {
						mode = RANDOM;
						cout << "Using random supersampling mode\n";
					}
	            } catch (YAML::TypedKeyNotFound<std::string> &e) {
					mode = GRID;
					cout << "Falling back to grid supersampling mode\n";
				}
                scene->setSuperSampling(factor, mode);
	        } catch (YAML::TypedKeyNotFound<std::string> &e) {
                cout << "Using default supersampling settings\n";
                scene->setSuperSampling();
            }

            // Read scene configuration options
			try {
				scene->setCamera(parseCamera(doc["Camera"]));
			} catch (YAML::TypedKeyNotFound<std::string> &e) {
				cout << "Falling back on the Eye parameter for camera specification\n";
				scene->setCamera(parseEyeAsCamera(doc["Eye"]));
			}
            //scene->setEye(parseTriple(doc["Eye"]));

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

void Raytracer::parseGoochParameters(const YAML::Node& node)
{
    scene->setGoochParameters(node["b"], node["y"], node["alpha"], node["beta"]);
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    cout << "Tracing..." << endl;
    Image img = scene->render();
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
