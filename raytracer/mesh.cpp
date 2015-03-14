#include <string>
#include <vector>

#include "glm.h"
#include "mesh.h"
#include "triangle.h"
#include "triple.h"

Mesh::Mesh(std::string filename, float scale)
{
    model = glmReadOBJ(const_cast<char*>(filename.c_str()));
	glmReverseWinding(model);
    glmUnitize(model);
    glmScale(model, scale);

    Triangle *t;
    Vector a, b, c;
    for (unsigned int i=0; i<model->numtriangles; ++i) {
		a = modelDataToPoint(model->vertices, model->triangles[i].vindices[0]);
		b = modelDataToPoint(model->vertices, model->triangles[i].vindices[1]);
		c = modelDataToPoint(model->vertices, model->triangles[i].vindices[2]);
        t = new Triangle(a, b, c);
        triangles.push_back(t);
    }
    cout << triangles.size() << endl;
}

Mesh::~Mesh()
{
    glmDelete(model);
}

Hit Mesh::intersect(const Ray &ray)
{
    Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
    bool has_hit = false;
    for (unsigned int i=0; i<triangles.size(); ++i) {
        Hit hit(triangles[i]->intersect(ray));
        if (hit.t < min_hit.t) {
            min_hit = hit;
            has_hit = true;
        }
    }
    if (has_hit)
        return min_hit;
    return Hit::NO_HIT();
}

Point Mesh::modelDataToPoint(float *array, unsigned int idx)
{
	return Vector(array[idx*3], array[idx*3+1], array[idx*3+2]);
}
