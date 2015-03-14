#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "glm.h"
#include "object.h"
#include "triangle.h"
#include "triple.h"

class Mesh : public Object
{
public:
    Mesh(std::string filename, float scale, Point pos);
    ~Mesh();

    virtual Hit intersect(const Ray &ray);
private:
	Point modelDataToPoint(float *array, unsigned int idx, Point offset=Point(0,0,0));

    GLMmodel *model;
    std::vector<SmoothTriangle*> triangles;
};

#endif // MESH_H
