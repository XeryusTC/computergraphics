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

typedef enum RENDER_MODE {
    PHONG,
    ZBUFFER,
    NORMAL,
    GOOCH,
} RENDER_MODE;

typedef enum SUPERSAMPLING_MODE {
	GRID,
	JITTER,
	RANDOM,
} SUPERSAMPLING_MODE;

// Cast shadow ray a bit above surface to prevent from detecting the surface
// being between itself and the light source
const double RECAST_OFFSET = 0.0000000001;


class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
	bool shadows;
    unsigned int maxRecursionDepth;
    int SSFactor;
	SUPERSAMPLING_MODE SSMode;
	Camera* camera;
    // Gooch rendering parameters
    float gooch_b, gooch_y, gooch_alpha, gooch_beta;

    // Change the render mode
    RENDER_MODE mode;
    Color renderPhong(Material *m, Point hit, Vector N, Vector V, Ray orgray,
            double t, unsigned int depth);
    Color renderGooch(Material *m, Point hit, Vector N, Vector V, Ray orgray,
            double t, unsigned int depth);
    Color renderZBuffer(Point hit);
    Color renderNormal(Vector N);
    // Other utility functions
    bool objectIsInShadow(Point hit, Light *light);
public:
    Color trace(const Ray &ray, unsigned int depth=0);
    Image render();
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
    void setMode(RENDER_MODE m);
	void setShadows(bool s);
    void setRecursionDepth(unsigned int d);
    void setSuperSampling(unsigned int factor=1, SUPERSAMPLING_MODE mode=GRID);
	void setCamera(Camera *c);
    void setGoochParameters(float bA, float yA, float alphaA, float betaA);
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
