#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>

#include "glm.h"
#include "object.h"
#include "triangle.h"

class Mesh : public Object
{
public:
    Mesh(std::string filename, float scale);
    ~Mesh();

    virtual Hit intersect(const Ray &ray);
private:
    GLMmodel *model;
    std::vector<Triangle*> triangles;
};

#endif // MESH_H
